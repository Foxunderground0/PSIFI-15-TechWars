
function pollRawData() {
    setInterval(function () {
        fetch('/rawData').then(response => response.text()).then(data => {
            document.getElementById('data').innerHTML += "&gt"+ data + "<br>";
        }
        );
    }, 1000);
}
pollRawData();
