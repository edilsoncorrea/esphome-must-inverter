# Must Solar Hybrid Inverter ESPHome Monitor

This repository provides an optimized **ESPHome YAML configuration** for monitoring and controlling **Must Solar Inverters**. By leveraging an **ESP32** (or **ESP8266**) via **Modbus** communication, you can gather comprehensive real-time data, including battery, PV, and inverter operational metrics. This solution integrates seamlessly with **Home Assistant** for streamlined solar energy management and smart home automation.

## Key Features

- **Real-time Solar Monitoring**  
  - **Battery Voltage**: Track battery voltage, current, and power to maintain optimal energy storage and usage.  
  - **PV Charger Metrics**: Monitor PV charger states, voltage, current, and power for accurate solar energy insights.  
  - **Inverter Status**: Check grid status, load power, bus voltage, and more for complete operational awareness.  
  - **Temperature Readings**: Gather temperature data (radiator, transformer, etc.) to ensure efficient cooling and system safety.

- **Detailed Energy Metrics**  
  - **Charging & Discharging**: Log and review accumulated power for charging and discharging cycles.  
  - **Buying & Selling Power**: Monitor power bought from and sold back to the grid for better cost management.  
  - **Load Consumption**: Measure load consumption and track long-term energy usage trends.  
  - **AC/DC Power & Energy**: Access aggregated AC and DC values in your **ESPHome** or **Home Assistant** dashboards.

- **Customizable Controls**  
  - **Energy Use Modes** (e.g., Solar-first, Utility-only) to optimize energy usage strategy.  
  - **Charger Source Priorities**: Tailor charger source (grid, solar, battery) for flexible power management.  
  - **Voltage Configurations**: Adjust voltage settings to suit specific battery and system requirements.

## Prerequisites

- **Hardware**  
  - **ESP32 Board**: Recommended for robust Wi-Fi/Bluetooth performance (ESP8266 also supported).  
    - Example: [WT32-ETH on AliExpress](https://www.aliexpress.com/item/1005006074972994.html)  
  - **RS485 to UART Board**: For reliable Modbus communication.  
    - Example: [AliExpress](https://www.aliexpress.com/item/1005001621746811.html)  
  - **USB Cable (Optional)**: Connect via USB if supported by your inverter model.  
    - Example: [AliExpress](https://www.aliexpress.com/item/1005002358382000.html) (works with PH(PV)18-5248 Pro, may vary for others)

- **Software**  
  - **ESPHome**: [Official website](https://esphome.io/)  
  - **Home Assistant** (Optional but recommended): For advanced automation and monitoring features.

## Connection Scheme

For a visual reference, see the wiring diagram below. Courtesy of [@Rogero-](https://github.com/Rogero-). More details and photos are in [Discussion #9](https://github.com/vladyspavlov/esphome-must-inverter/discussions/9#discussioncomment-11593361).

<img src="https://github.com/user-attachments/assets/f21a1027-218e-4f2c-9b26-354641475daa" alt="Connection scheme picture" width="500" />

## Tested Models

This configuration supports all **PV/PH18** and **PV19** series **Must Solar Inverters**. Community-confirmed models include:

- **PV18-5248 Pro**  
- **PH18-5248 Pro**  
- **PV18-3024 VHM** (Thanks to [@gjury](https://github.com/gjury))  
- **PV18-3224 VPM**  
- **PV18-1012 VPM** (Thanks to [@sstepane](https://github.com/sstepane))  
- **PV19-6248 EXP** (Thanks to [@sergeysaley](https://github.com/sergeysaley))

## Demo Video

Watch a live demonstration on [YouTube](https://youtu.be/0Ef8nHztPZQ).

## Available Configurations

| File / Folder | Board | Display | Description |
|---|---|---|---|
| `esp32-pv18.yaml` | ESP32 | — | Full PV18 monitor |
| `esp32-pv19.yaml` | ESP32 | — | Full PV19 monitor |
| `esp32-pv29-*.yaml` | ESP32 | — | PV29 variants (flow control, scanner, etc.) |
| `esp8266-pv19.yaml` | ESP8266 | — | Lightweight PV19 (ESP8266) |
| `esp01-pv18.yaml` | ESP-01 | — | Minimal PV18 (ESP-01) |
| [`esp32-c3-pv19-oled/`](esp32-c3-pv19-oled/README.md) | ESP32-C3 | SSD1306 0.96" OLED | PV19 monitor with I2C OLED display |
| `esp32-s3-pv18-usb.yaml` | ESP32-S3 | — | PV18 via USB (ESP32-S3) |

---

## Usage

1. **Clone this Repository**: Get all YAML files and relevant assets.  
2. **Edit the YAML Configuration**: Replace `wifi_ssid`, `wifi_password`, `api_key`, and `ota_password` with your actual credentials.  
3. **Flash Your ESP32**: Use **ESPHome** to upload the YAML configuration to your board.  
4. **Hardware Connection**: Plug the ESP32 into the inverter’s communication port (RS485 or USB, depending on your inverter model).  
5. **Monitor Data**: View real-time metrics in **Home Assistant** or the **ESPHome** dashboard.

## Notes

- You can adjust **logging levels** (`INFO`, `DEBUG`) in the YAML file to balance performance with diagnostic detail.  
- Compatibility may vary across inverter models; always double-check your inverter’s documentation.

## License

This project is released under the **MIT License**. You are free to use, modify, and distribute this code as needed.

## References

- [ESPHome Must Inverter Gist](https://gist.github.com/vladyspavlov/5ac21cb58923482eff8e7bbb2d0854b3)

