var enter_ok = 0;
var loaded_old_dialogue = 0;
var new_dialogue_id_count = 0;

function pollRawData() {
    setInterval(function () {
        fetch('/rawData').then(response => response.text()).then(data => {
            document.getElementById('data').innerHTML += "&gt" + data + "<br>";
        }
        );
    }, 1000);
}
//pollRawData();

// Periodically check the console ready flag
var checkFlagInterval = setInterval(function () {
    if (enter_ok === 1) {
        getCardyChat();
        clearInterval(checkFlagInterval); // Stop checking once flag is 1
        console.log("Ready");

        document.getElementById("input").addEventListener("keypress", function (event) {
            if (event.key === "Enter" && event.target.value.trim() !== "") {
                event.preventDefault();
                var text = event.target.value;
                var xhr = new XMLHttpRequest();
                var url = "/entered";
                url += "?command=" + encodeURIComponent(text);
                xhr.open("GET", url, true);
                xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4) {
                        if (xhr.status == 200) {
                            // Request was successful, and response is available
                            var responseText = xhr.responseText;
                            console.log("GET request sent successfully. Response:", responseText);
                            document.getElementById('data').innerHTML += "&gt" + "  " + text + "<br>" + responseText + "<br>";
                        } else {
                            // Request failed
                            console.error("GET request failed with status:", xhr.status);
                        }
                    }
                };
                xhr.send();
                event.target.value = "";
            }
        });
    }
}, 100); // Check every 100 milliseconds

let typingInProgress = false;

function getCardyChat() {
    cardyChatInterval = setInterval(function () {
        if (typingInProgress) {
            // If typing is already in progress, skip this interval
            return;
        }

        fetch('/dialogReady')
            .then(response => response.text())
            .then(async data => {
                if (data == "1") {
                    if (loaded_old_dialogue == 0) {
                        await fetch('/pastDialogue')
                            .then(response => response.text())
                            .then(data => {
                                console.log("HEREEEE");
                                data = data.split("\n");
                                data.forEach((data) => {
                                    document.getElementById('pastDialogue').innerHTML += data + "<br>";
                                    console.log(data);
                                });
                            });
                        loaded_old_dialogue = 1;
                    } else if (loaded_old_dialogue == 1) {
                        //clearInterval(cardyChatInterval);
                        await fetch('/latestDialogue')
                            .then(response => response.text())
                            .then(data => {
                                typingInProgress = true; // Set the flag to true
                                document.getElementById('data').innerHTML += "<span id=\"dialogue_" + new_dialogue_id_count + "\" class=\"purple\">" + "</span><br>";
                                console.log(data);
                                typeDialogue("dialogue_" + new_dialogue_id_count, data);
                                new_dialogue_id_count += 1;
                            });
                    }
                }
            });
    }, 100);
}

function typeDialogue(id, data) {
    const element = document.getElementById(id);
    let charIndex = 0;
    element.innerHTML = ""; // Clear the element initially

    function typeText() {
        if (charIndex < data.length) {
            const char = data.charAt(charIndex);
            element.innerHTML += "<span class=\"purple\">" + char + "</span>"; // Add each character within the same <span>
            charIndex++;
            setTimeout(typeText, 50);
        } else {
            typingInProgress = false; // Set the flag to false when typing is complete
        }
    }

    typeText();
}