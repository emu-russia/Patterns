# Patterns

Utility for searching and marking up standard cells.

## Purpose

With this tool you can quickly match standard cells from original image against pattern library and export it as XML file.

But the utility also may be used for any other similar purposes (Maya glyphs e.g.).

## Workspace

![/imgstore/workspace.png](/imgstore/workspace.png)

Working environment consists of two windows and a status bar.

Source image is displayed on the left window overlaid by found standard cells.

Patterns Database is shown on the right side (standard cells library)

The status bar displays the current state of the work environment.

Tool functionality can be represented as follows:

![/imgstore/layers.png](/imgstore/layers.png)

- Patterns from Database are placed over Added Patterns Layer corresponding to source image
- Added patterns coords and names are exported to simple XML file for further usage in Deroute tool

See the `UserManual` section for more.
