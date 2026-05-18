
# Calliope Blocks Extension (pxt-blocks)

MakeCode/PXT extension that exposes a BLE service and matching USB serial
protocol so the [Calliope Campus](https://github.com/calliope-edu/calliope-campus)
Blocks editor can drive sensors, actuators, and messaging on a Calliope mini.

Forked from upstream [Microbit More v2](https://github.com/microbit-more/pxt-mbit-more-v2);
the wire protocol (GATT UUIDs, channel ids, SFD framing) is unchanged so existing
clients keep working.

## Use as Extension

Add as an **extension** in MakeCode/Calliope for `calliopemini`:

* open the Calliope MakeCode editor
* click on **New Project**
* click on **Extensions** under the gearwheel menu
* import this repository

## Blocks

The runtime registers a "Blocks" namespace with `on number $numberData with
label $label`, `on text $textData with label $label`, `send number with
label`, and `send text with label` blocks for messaging between the editor
and the device.

#### Metadata (used for search, rendering)

* for PXT/calliopemini
<script src="https://makecode.com/gh-pages-embed.js"></script><script>makeCodeRender("{{ site.makecode.home_url }}", "{{ site.github.owner_name }}/{{ site.github.repository_name }}");</script>
