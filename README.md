# Mobile UI Example

A minimal Geode mod for Geometry Dash demonstrating a custom-styled mobile UI popup. Built against:

- Geode: `5.3.0`
- GD (Android): `2.2081`

## What it does

Adds a button to the **pause menu** (shown when you pause inside a level). Tapping it opens a popup containing:

- A gold accent divider under the title
- A soft background panel behind the content
- A description label that automatically word-wraps instead of running off the edge
- A large, touch-friendly button (gold-styled) that shows a notification
- A standard toggle switch

All controls are arranged automatically with Geode's `ColumnLayout`.

## Project layout

```
mobile-ui-example/
├── mod.json                     # Mod metadata (id, version, target Geode/GD versions)
├── CMakeLists.txt                # CMake build config
├── about.md                      # In-game "About" page
├── src/
│   └── main.cpp                  # Popup + PauseLayer hook
└── .github/workflows/build.yml   # CI: builds Android32 + Android64 and packages a .geode file
```

## Building locally

Requires the [Geode CLI](https://docs.geode-sdk.org/getting-started/) and SDK installed, with `GEODE_SDK` set in your environment.

```bash
geode build
```

The resulting `.geode` file will be in the `build/` directory (or auto-installed to GD if you've set up a profile with the CLI).

## Building with GitHub Actions

Push this repo to GitHub (or fork/clone it there) and the included workflow will build Android32 and Android64 automatically on every push, combining them into a single `.geode` file available as a workflow artifact under **Actions → (latest run) → Build Output**.

## Notes

- The button is added to the pause menu's `right-button-menu` (via [`geode.node-ids`](https://geode-sdk.org/mods/geode.node-ids)). If that container isn't found for any reason, the code falls back to placing the button manually in the top-right corner, so it never silently disappears.
