
var enter_ok = 0;

function pollRawData() {
    setInterval(function () {
        fetch('/rawData').then(response => response.text()).then(data => {
            document.getElementById('data').innerHTML += "&gt"+ data + "<br>";
        }
        );
    }, 1000);
}
//pollRawData();


    // Periodically check the flag
var checkFlagInterval = setInterval(function() {
    if (enter_ok === 1) {
        clearInterval(checkFlagInterval); // Stop checking once flag is 1
        console.log("Ready");

        document.getElementById("input").addEventListener("keypress", function(event) {
            if (event.key === "Enter" && event.target.value.trim() !== "") {
              event.preventDefault();
              var text = event.target.value;
              var xhr = new XMLHttpRequest();
              var url = "/entered";
              url += "?command=" + encodeURIComponent(text);
              xhr.open("GET", url, true);
              xhr.onreadystatechange = function() {
                if (xhr.readyState == 4 && xhr.status == 200) {
                  console.log("GET request sent successfully.");
                }
              };
              xhr.send();
              event.target.value = "";
            }
        });
    }
}, 100); // Check every 100 milliseconds
  