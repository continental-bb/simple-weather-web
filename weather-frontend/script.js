// script.js
async function fetchWeather(lat, lon) {
    try {
        const res = await fetch(`/weather?lat=${lat}&lon=${lon}`);
        const data = await res.json();

        const temp = Math.round(data.main.temp);
        const desc = data.weather[0].description;
        const city = data.name;

        document.getElementById('temperature').textContent = `${temp} °C`;
        document.getElementById('description').textContent = desc;
        document.getElementById('location').textContent = city;

        // Set background based on temperature
        if (temp < 10) {
            document.body.style.backgroundImage = "url('images/cold.jpg')";
        } else if (temp < 25) {
            document.body.style.backgroundImage = "url('images/cloudy.jpg')";
        } else {
            document.body.style.backgroundImage = "url('images/sunny.jpg')";
        }
    } catch (err) {
        document.getElementById('location').textContent = "Error fetching weather";
        console.error(err);
    }
}

// Get geolocation
if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(
        pos => fetchWeather(pos.coords.latitude, pos.coords.longitude),
        err => console.error(err)
    );
} else {
    document.getElementById('location').textContent = "Geolocation not supported";
}