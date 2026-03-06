message = {
    "id": "0x100",
    "signals": [
        {"name": "temperature", "type": "float", "length": 10, "comment": "the ambient temperature"},
        {"name": "humidity", "type": "uint8_t", "length": 7, "comment": "the ambient humidity percentage"},
        {"name": "dht_sensor_status", "type": "uint8_t", "length": 2, "comment": "the dht sensor status"},
        {"name": "flow_rate", "type": "uint16_t", "length": 9, "comment": "the flow rate in milliliter per second"},
        {"name": "flow_meter_sensor_status", "type": "uint8_t", "length": 2, "comment": "the flow meter sensor status"},
        {"name": "light_intensity", "type": "uint8_t", "length": 7, "comment": "the light intensity percentage"},
        {"name": "light_intensity_sensor_status", "type": "uint8_t", "length": 2, "comment": "the light intensity sensor status"},
        {"name": "water_level", "type": "uint8_t", "length": 7, "comment": "the water level percentage"},
        {"name": "water_level_sensor_status", "type": "uint8_t", "length": 2, "comment": "the water level sensor status"},
        {"name": "soil_moisture", "type": "uint8_t", "length": 7, "comment": "the soil moisture percentage"},
        {"name": "soil_moisture_sensor_status", "type": "uint8_t", "length": 2, "comment": "the soil moisture sensor status"},
    ]
}

signals = message["signals"]

# Calculate dynamic column widths
name_width = max(len("Signal"), *(len(s["name"]) for s in signals))
type_width = max(len("Type"), *(len(s["type"]) for s in signals))
length_width = max(len("Length"), *(len(str(s["length"])) for s in signals))
desc_width = max(len("Description"), *(len(s["comment"]) for s in signals))

# Header
print(f"{'Signal':<{name_width}} | "
      f"{'Type':<{type_width}} | "
      f"{'Length':<{length_width}} | "
      f"{'Description':<{desc_width}}")

print("-" * (name_width + type_width + length_width + desc_width + 9))

# Rows
for s in signals:
    print(f"{s['name']:<{name_width}} | "
          f"{s['type']:<{type_width}} | "
          f"{s['length']:<{length_width}} | "
          f"{s['comment']:<{desc_width}}")