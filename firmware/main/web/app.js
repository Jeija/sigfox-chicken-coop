(function() {
	let latestStatus = null;

	function setMessage(targetId, message, kind) {
		const el = document.getElementById(targetId);
		el.textContent = message || "";
		el.className = el.className.replace(/\s(success|error)/g, "");
		if (kind) {
			el.classList.add(kind);
		}
	}

	function formatAutoclose(hour) {
		return hour === 25 ? "Aus" : String(hour).padStart(2, "0") + ":00";
	}

	function boolLabel(value, truthy, falsy) {
		return value ? truthy : falsy;
	}

	function setMetric(id, value) {
		document.getElementById(id).textContent = value;
	}

	function fillForm(data) {
		document.getElementById("currentTime").value =
			String(data.timeHours).padStart(2, "0") + ":" + String(data.timeMinutes).padStart(2, "0");
		document.getElementById("alarmTime").value =
			String(data.alarmHours).padStart(2, "0") + ":" + String(data.alarmMinutes).padStart(2, "0");
		document.getElementById("autocloseHour").value = String(data.autocloseHours);
	}

	function renderStatus(data) {
		latestStatus = data;
		setMetric("metricTime", data.time);
		setMetric("metricWakeReason", "Weckgrund: " + data.wakeReason);
		setMetric("metricDesiredState", data.desiredDoorState);
		setMetric("metricDesiredReached", "Erreicht: " + boolLabel(data.desiredDoorStateReached, "ja", "nein"));
		setMetric("metricMotorState", data.motorState);
		setMetric("metricButtonState", "Tasten: " + data.buttonState);
		setMetric("metricReed", boolLabel(data.reedClosed, "Zu", "Offen"));
		setMetric(
			"metricDoorFlags",
			"Oben: " + boolLabel(data.doorUpReached, "ja", "nein") + ", Unten: " + boolLabel(data.doorDownReached, "ja", "nein")
		);
		setMetric("metricBattery", data.batteryVoltage.toFixed(2) + " V");
		setMetric("metricPower", "Motor: " + data.motorPower.toFixed(3) + " W / " + data.motorCurrent.toFixed(3) + " A");
		setMetric("metricClimate", data.temperature.toFixed(1) + " °C");
		setMetric(
			"metricClimateDetail",
			data.pressure.toFixed(0) + " Pa, " + data.humidity.toFixed(1) + " %RH"
		);
		setMetric("metricAlarm", data.alarm);
		setMetric("metricAutoclose", "Auto-Close: " + formatAutoclose(data.autocloseHours));
		setMetric(
			"metricSolar",
			boolLabel(data.solarCharging, "Lädt", "Leerlauf") + (data.solarFault ? " / Fehler" : " / OK")
		);
		setMetric("metricSigfoxAuthority", "Sigfox: " + (data.sigfoxAuthoritative ? "aktiv" : "aus"));
		fillForm(data);
		setMessage("statusBanner", "Status geladen.", "success");
	}

	async function fetchStatus() {
		const response = await fetch("/api/status", { cache: "no-store" });
		if (!response.ok) {
			throw new Error("Status konnte nicht geladen werden");
		}
		return response.json();
	}

	async function loadStatus() {
		try {
			const data = await fetchStatus();
			renderStatus(data);
		} catch (error) {
			setMessage("statusBanner", error.message, "error");
		}
	}

	function buildConfigPayload(mode) {
		const parts = ["mode=" + mode];
		if (mode === "save") {
			parts.push("current_time=" + document.getElementById("currentTime").value);
			parts.push("alarm_time=" + document.getElementById("alarmTime").value);
			parts.push("autoclose_hour=" + document.getElementById("autocloseHour").value);
		}
		return parts.join("&");
	}

	function setBusy(button, busy) {
		button.disabled = busy;
	}

	async function reboot(mode, buttonId) {
		const button = document.getElementById(buttonId);
		setBusy(button, true);
		setMessage("statusBanner", mode === "save" ? "Speichere und starte neu..." : "Starte neu...", "");

		try {
			const response = await fetch("/api/reboot", {
				method: "POST",
				headers: {
					"Content-Type": "application/x-www-form-urlencoded"
				},
				body: buildConfigPayload(mode)
			});
			const text = await response.text();
			if (!response.ok) {
				throw new Error(text || "Neustart fehlgeschlagen");
			}

			const payload = JSON.parse(text);
			setMessage("statusBanner", payload.message, "success");
		} catch (error) {
			setBusy(button, false);
			setMessage("statusBanner", error.message, "error");
		}
	}

	async function triggerSigfoxSync() {
		const button = document.getElementById("sigfoxButton");
		setBusy(button, true);
		setMessage("statusBanner", "Starte Sigfox-Sync...", "");

		try {
			const response = await fetch("/api/sigfox-sync", { method: "POST" });
			const text = await response.text();
			if (!response.ok) {
				throw new Error(text || "Sigfox-Sync fehlgeschlagen");
			}
			const payload = JSON.parse(text);
			setMessage("statusBanner", payload.message, "success");
			await loadStatus();
		} catch (error) {
			setMessage("statusBanner", error.message, "error");
		} finally {
			setBusy(button, false);
		}
	}

	function startUpload() {
		const input = document.getElementById("otafile");
		const button = document.getElementById("uploadButton");
		const fill = document.getElementById("progressFill");

		if (!input.files.length) {
			alert("Bitte zuerst eine Datei wählen.");
			return;
		}

		input.disabled = true;
		button.disabled = true;
		fill.style.width = "0%";
		setMessage("otaStatus", "Upload läuft...", "");

		const xhr = new XMLHttpRequest();
		xhr.onreadystatechange = function() {
			if (xhr.readyState !== 4) {
				return;
			}

			if (xhr.status === 200) {
				document.open();
					document.write(xhr.responseText);
					document.close();
				} else if (xhr.status === 0) {
					alert("Verbindung wurde unerwartet beendet.");
					location.reload();
				} else {
					alert(xhr.status + " Fehler\n" + xhr.responseText);
					location.reload();
				}
			};

		xhr.upload.onprogress = function(event) {
			if (!event.lengthComputable) {
				return;
			}
			const percent = (event.loaded / event.total * 100).toFixed(0);
			fill.style.width = percent + "%";
			setMessage("otaStatus", "Upload: " + percent + "%", "");
		};

		xhr.open("POST", "/update", true);
		xhr.send(input.files[0]);
	}

	function installHandlers() {
		document.getElementById("refreshButton").addEventListener("click", loadStatus);
		document.getElementById("sigfoxButton").addEventListener("click", triggerSigfoxSync);
		document.getElementById("saveRebootButton").addEventListener("click", function() {
			reboot("save", "saveRebootButton");
		});
		document.getElementById("discardRebootButton").addEventListener("click", function() {
			reboot("discard", "discardRebootButton");
		});
		document.getElementById("uploadButton").addEventListener("click", startUpload);
		document.getElementById("configForm").addEventListener("submit", function(event) {
			event.preventDefault();
		});
	}

	window.addEventListener("DOMContentLoaded", function() {
		installHandlers();
		loadStatus();
	});
})();
