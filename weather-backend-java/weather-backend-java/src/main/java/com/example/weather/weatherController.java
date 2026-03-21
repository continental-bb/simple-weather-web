package com.example.weather;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;
import org.springframework.http.ResponseEntity;

@RestController
@CrossOrigin
public class weatherController {
    @Value("${api.key}")
    private String apiKey;

    @GetMapping("/weather")
    public ResponseEntity<String> getWeather(@RequestParam double lat, @RequestParam double lon) {
        String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + apiKey + "&units=metric";
        RestTemplate restTemplate = new RestTemplate();
        try {
            String response = restTemplate.getForObject(url, String.class);
            return ResponseEntity.ok(response);
        } catch (Exception e) {
            return ResponseEntity.status(500).body("{\"error\": \"Failed to fetch weather data\"}");
        }
    }
}