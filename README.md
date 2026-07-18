# Mobile UI Example

A minimal Geode mod for Geometry Dash demonstrating a simple mobile UI popup. Built against:

- Geode: `5.3.0`
- GD (Android): `2.2081`

## What it does

Adds a button to the bottom menu of the main menu. Tapping it opens a popup containing:

- A scaled label
- A large, touch-friendly button that shows a notification
- A standard toggle switch

All controls are arranged automatically with Geode's `ColumnLayout`.

## Project layout

```
mobile-ui-example/
├── mod.json                     # Mod metadata (id, version, target Geode/GD versions)
├── CMakeLists.txt                # CMake build config
├── about.md                      # In-game "About" page
├── src/
│   └── main.cpp                  # Popup + MenuLayer hook
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

- This mod depends on [`geode.node-ids`](https://geode-sdk.org/mods/geode.node-ids) to reliably find the main menu's bottom button row (`bottom-menu`). It'll be pulled in automatically as a dependency.
- Only targets Android since that's the platform specified — add `win`/`mac`/`ios` versions to `mod.json`'s `gd` field and extend the build matrix in `build.yml` if you want other platforms too.
