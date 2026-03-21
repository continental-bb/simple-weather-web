package com.example.weather;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;
@RestController
@CrossOrigin
public class weatherController {
    @Value("${api.key}")
    private String apiKey;

    @GetMapping("/weather")
    public String getWeather() {
        // Here you would typically call an external weather API using the apiKey
        // For demonstration purposes, we'll return a dummy response
        return "Key Start with:" +apiKey.substring(0,3 + "****";
    }
}