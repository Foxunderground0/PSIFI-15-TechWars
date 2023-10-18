function pollRawData() {
    setInterval(function () {
        fetch('192.168.1.14/rawData').then(response => response.text()).then(data => {
            document.getElementById('ZZZ').textContent = data;
        }
        );
    }, 1000);
}
pollRawData();