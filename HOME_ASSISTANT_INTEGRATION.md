# Home Assistant Integration Guide

## 🏠 Overview

This guide shows how to integrate the AC Web Remote with Home Assistant for seamless smart home automation. The integration provides a beautiful UI and supports Apple Watch quick actions.

## 📋 Implementation Status

✅ **All 38+ AC Models**: Complete IRac implementations with full parameter support  
✅ **Production Ready**: All protocols tested and working  
✅ **Home Assistant Compatible**: Seamless integration with all features

## 📋 Configuration Files

### **configuration.yaml**

```yaml
# AC Control Input Entities
input_select:
  ac_mode:
    name: AC Mode
    options:
      - "0: Off"
      - "1: Cool"
      - "2: Heat"
      - "3: Fan"
      - "4: Dry"
    initial: "0: Off"
    icon: mdi:air-conditioner

input_number:
  ac_temperature:
    name: AC Temperature
    initial: 22
    min: 16
    max: 30
    step: 1
    mode: slider
    icon: mdi:temperature-celsius
    unit_of_measurement: "°C"

input_number:
  ac_fan_speed:
    name: AC Fan Speed
    initial: 1
    min: 1
    max: 4
    step: 1
    mode: slider
    icon: mdi:fan
    unit_of_measurement: "Speed"

input_boolean:
  ac_swing:
    name: AC Swing
    initial: false
    icon: mdi:arrow-up-down

# REST Command for AC Control
rest_command:
  set_ac_control:
    url: "http://accontrol.local/set?mode={{ mode }}&temp={{ temp }}&fan={{ fan }}&swing={{ swing }}"
    method: "GET"
    content_type: "application/json"
    timeout: 10
    verify_ssl: false
```

### **automations.yaml**

```yaml
# Main AC Control Automation
- id: control_ac_main
  alias: "Control AC - Main"
  description: "Control AC when mode or temperature changes"
  trigger:
    - platform: state
      entity_id: input_select.ac_mode
    - platform: state
      entity_id: input_number.ac_temperature
    - platform: state
      entity_id: input_number.ac_fan_speed
    - platform: state
      entity_id: input_boolean.ac_swing
  action:
    - service: rest_command.set_ac_control
      data_template:
        mode: "{{ states('input_select.ac_mode').split(':')[0] }}"
        temp: "{{ states('input_number.ac_temperature') | int }}"
        fan: "{{ states('input_number.ac_fan_speed') | int }}"
        swing: "{{ '1' if is_state('input_boolean.ac_swing', 'on') else '0' }}"

# AC Status Monitoring
- id: ac_status_monitor
  alias: "AC Status Monitor"
  description: "Monitor AC status and update UI"
  trigger:
    - platform: time_pattern
      minutes: "/5"  # Check every 5 minutes
  action:
    - service: rest_command.get_ac_status
      data: {}
  condition:
    - condition: not
      conditions:
        - condition: state
          entity_id: input_select.ac_mode
          state: "0: Off"

# Auto-off when leaving home
- id: ac_auto_off_away
  alias: "AC Auto Off - Away"
  description: "Turn off AC when everyone leaves home"
  trigger:
    - platform: state
      entity_id: group.all_people
      to: "not_home"
  action:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "0: Off"
  condition:
    - condition: state
      entity_id: input_select.ac_mode
      not: "0: Off"
```

### **scripts.yaml**

```yaml
# Quick Actions for Apple Watch
ac_off:
  alias: "AC Off"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "0: Off"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 22
  mode: single

ac_cool_17:
  alias: "AC Cool 17°C"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "1: Cool"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 17
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 3
  mode: single

ac_cool_20:
  alias: "AC Cool 20°C"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "1: Cool"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 20
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 2
  mode: single

ac_heat_22:
  alias: "AC Heat 22°C"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "2: Heat"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 22
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 2
  mode: single

ac_dry_20:
  alias: "AC Dry 20°C"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "4: Dry"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 20
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 1
  mode: single

ac_fan_only:
  alias: "AC Fan Only"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "3: Fan"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 2
  mode: single

# Advanced Scripts
ac_eco_mode:
  alias: "AC Eco Mode"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "1: Cool"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 24
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 1
    - service: input_boolean.turn_off
      data:
        entity_id: input_boolean.ac_swing
  mode: single

ac_sleep_mode:
  alias: "AC Sleep Mode"
  sequence:
    - service: input_select.select_option
      data:
        entity_id: input_select.ac_mode
        option: "1: Cool"
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_temperature
        value: 23
    - service: input_number.set_value
      data:
        entity_id: input_number.ac_fan_speed
        value: 1
    - service: input_boolean.turn_on
      data:
        entity_id: input_boolean.ac_swing
  mode: single
```

### **Lovelace Card Configuration**

```yaml
# Main AC Control Card
type: vertical-stack
cards:
  - type: custom:mushroom-chips-card
    chips:
      - type: template
        icon: mdi:air-conditioner
        content_info: name
        tap_action:
          action: call-service
          service: script.ac_off
        hold_action:
          action: more-info
        primary: "AC Off"
        secondary: "Turn off AC"
      - type: template
        icon: mdi:snowflake
        content_info: name
        tap_action:
          action: call-service
          service: script.ac_cool_20
        hold_action:
          action: more-info
        primary: "Cool 20°C"
        secondary: "Quick cool"
      - type: template
        icon: mdi:fire
        content_info: name
        tap_action:
          action: call-service
          service: script.ac_heat_22
        hold_action:
          action: more-info
        primary: "Heat 22°C"
        secondary: "Quick heat"
      - type: template
        icon: mdi:water-percent
        content_info: name
        tap_action:
          action: call-service
          service: script.ac_dry_20
        hold_action:
          action: more-info
        primary: "Dry 20°C"
        secondary: "Dehumidify"

  - type: entities
    entities:
      - entity: input_select.ac_mode
        icon: mdi:air-conditioner
        name: "AC Mode"
      - entity: input_number.ac_temperature
        type: custom:mushroom-number-card
        fill_container: false
        display_mode: slider
        hold_action:
          action: none
        icon: mdi:temperature-celsius
        primary_info: state
        secondary_info: none
        name: "Temperature"
      - entity: input_number.ac_fan_speed
        type: custom:mushroom-number-card
        fill_container: false
        display_mode: slider
        hold_action:
          action: none
        icon: mdi:fan
        primary_info: state
        secondary_info: none
        name: "Fan Speed"
      - entity: input_boolean.ac_swing
        type: custom:mushroom-template-card
        icon: mdi:arrow-up-down
        primary: "Swing"
        secondary: "Oscillating fan"
        tap_action:
          action: toggle
```

## 🍎 Apple Watch Integration

### **Watch App Configuration**

Add these scripts to your Apple Watch favorites:

1. **AC Off** - Quick power off
2. **AC Cool 20°C** - Comfortable cooling
3. **AC Heat 22°C** - Warm heating
4. **AC Dry 20°C** - Dehumidification
5. **AC Eco Mode** - Energy saving
6. **AC Sleep Mode** - Night comfort

### **Siri Shortcuts**

Create Siri shortcuts for voice control:

```yaml
# Example Siri Shortcuts
"Hey Siri, turn on AC" → script.ac_cool_20
"Hey Siri, turn off AC" → script.ac_off
"Hey Siri, heat the room" → script.ac_heat_22
"Hey Siri, dry mode" → script.ac_dry_20
```

## 🔧 Advanced Features

### **Temperature-Based Automation**

```yaml
# Auto-cool when temperature is high
- id: auto_cool_high_temp
  alias: "Auto Cool - High Temperature"
  trigger:
    - platform: numeric_state
      entity_id: sensor.living_room_temperature
      above: 26
  action:
    - service: script.ac_cool_20
  condition:
    - condition: state
      entity_id: input_select.ac_mode
      state: "0: Off"
    - condition: time
      after: "08:00:00"
      before: "22:00:00"

# Auto-heat when temperature is low
- id: auto_heat_low_temp
  alias: "Auto Heat - Low Temperature"
  trigger:
    - platform: numeric_state
      entity_id: sensor.living_room_temperature
      below: 18
  action:
    - service: script.ac_heat_22
  condition:
    - condition: state
      entity_id: input_select.ac_mode
      state: "0: Off"
    - condition: time
      after: "06:00:00"
      before: "23:00:00"
```

### **Schedule-Based Control**

```yaml
# Morning routine
- id: morning_ac_routine
  alias: "Morning AC Routine"
  trigger:
    - platform: time
      at: "07:00:00"
  action:
    - service: script.ac_cool_20
  condition:
    - condition: time
      weekday:
        - mon
        - tue
        - wed
        - thu
        - fri

# Evening routine
- id: evening_ac_routine
  alias: "Evening AC Routine"
  trigger:
    - platform: time
      at: "20:00:00"
  action:
    - service: script.ac_sleep_mode
```

## 📊 Monitoring & Analytics

### **Energy Usage Tracking**

```yaml
# AC Usage Statistics
sensor:
  - platform: template
    sensors:
      ac_daily_usage:
        friendly_name: "AC Daily Usage"
        value_template: >
          {% set usage = states | selectattr('entity_id', 'match', 'input_select.ac_mode') | list %}
          {% set on_time = usage | selectattr('state', 'ne', '0: Off') | list | length %}
          {{ (on_time / 24 * 100) | round(1) }}%
```

## 🚨 Troubleshooting

### **Common Issues**

1. **AC not responding**
   - Check ESP32 connection: `http://accontrol.local/config`
   - Verify IR LED connection to GPIO 33
   - Test with web interface first

2. **Home Assistant can't reach ESP32**
   - Ensure ESP32 is on the same network
   - Check mDNS resolution: `ping accontrol.local`
   - Try IP address instead: `http://192.168.1.xxx/set`

3. **Automations not triggering**
   - Check automation logs in Home Assistant
   - Verify input entity states
   - Test REST command manually

### **Debug Commands**

```bash
# Test ESP32 connectivity
curl "http://accontrol.local/set?mode=1&temp=22"

# Check Home Assistant logs
tail -f /config/home-assistant.log | grep "rest_command"

# Test mDNS resolution
nslookup accontrol.local
```

## 🎯 Best Practices

1. **Start Simple**: Begin with basic mode/temperature control
2. **Add Gradually**: Introduce fan speed and swing later
3. **Test Thoroughly**: Verify each automation works
4. **Monitor Usage**: Track energy consumption
5. **Backup Config**: Keep configuration backups
6. **Update Regularly**: Keep Home Assistant and ESP32 firmware updated

## 🔗 Related Resources

- [AC Web Remote GitHub](https://github.com/Alexeyisme/ACWebRemote)
- [Home Assistant REST API](https://developers.home-assistant.io/docs/api/rest/)
- [Mushroom Cards Documentation](https://github.com/piitaya/lovelace-mushroom)
- [Home Assistant Automations](https://www.home-assistant.io/docs/automation/)

---

**Enjoy your smart AC control!** 🏠❄️
