# Pattern Matching Utility

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

Controls:

- Right mouse: scrolling of source image and added patterns
- Left button on pattern: pattern selection and drag&drop
- Left mouse button on delete button (<img src="/imgstore/remove_button.png"/>): remove pattern
- Double click on pattern: rotate against flip/mirror flags
- Left mouse button on empty space: selection box:

![/imgstore/add_pattern.png](/imgstore/add_pattern.png)

Program will sort patterns database automatically and displays only some of them, which match to selected region (according to Lambda/Delta)

Clicking on pattern from database will add specified pattern at selected box coordinates

«Flip» checkbox allow to add flipped patterns. «Mirror» checkbox allow to add mirrored patterns.

## Lambda / Delta Parameters

**Lambda** – the main characteristic parameter of standard cell. It defines the minimum topological size of unit.

This parameter is used for easy scale comparison of the original image and the image of the pattern from the database (they may not fully reflect the scale).

To do this, Lambda parameter is assigned to original image and work with the width and height is carried out not in absolute units (pixels), but in relative (lambda).

![/imgstore/lambda_delta.png](/imgstore/lambda_delta.png)

**Delta** – is the relative error in the measurement of Lambda, which is used to determine the size of the cell. Delta is used to retrieve suitable patterns from the database with minor variations of pattern dimensions.

## Source Image layer

Source image is loading here. Only supported format is Jpeg.

## Added Patterns layer

List of added patterns saved here.

## Patterns Database

Patterns library is loaded from database. Pattern database are text files (patterns_db.txt), format is shown below:

```
# Patterns database

# Pattern Syntax:
# pattern name, source_lamda, pcount, ncount, image_path

# Inverters and Buffers

pattern NOT1, 8.0, 1, 1, "patterns_db/NOT1.jpg"

# Logic Operations

# Multiplexers

pattern IMUX, 6.5, 4, 4, "patterns_db/IMUX.jpg"

# With Clock

pattern nDFF, 4.0, 12, 12, "patterns_db/nDFF.jpg"

# Adder / Multiplier

# Misc

# EOF
```

Lines starting with `#` counted as comments.

The format of the description of the pattern is as follows:

`pattern` (keyword) `PATTERN_NAME` (pattern name), `LAMBDA`, (Lambda parameter for cell, float), `PCOUNT` (count of p-type transistors), `NCOUNT` (count of n-type transistors), `"Path_to_pattern_image.jpg"` (path to pattern image, relative or absolute)

Current pattern database is saved in workspace savestate files (.WRK) among with other workspace environment.

**_The path to the original image is saved relative to .WRK file._**

## Main menu

File → Load Source Image: Loads source image Jpeg. If the image has already been loaded, the previous image is unloaded.

File → Load Pattern Database: Load new pattern database. Old pattern database is wiped out. Missing patterns which are already present in workspace are no longer displayed (but remains virtually present)

Convenient to use if you want to update current workspace by a new pattern database.

File → Save Patterns as Image: Saves a composite image of the original image with overlayed added patterns.

File → Save Patterns as Text: Saves added patterns in a formatted text file.

Workspace → Load Workspace: Loads a saved state of the working environment from .WRK file.

Workspace → Save Workspace: Saves current state of the workspace in the .WRK file

Workspace → Remove all added patterns: Removes all added patterns from patterns layer

Workspace → Ensure Visible: Scroll workspace automatically to make sure selected pattern is visible

Workspace → Show debug profile info: Displays debugging information of the profiler (speed of individual procedures)

## Settings

![/imgstore/settings.png](/imgstore/settings.png)

In Settings you set the Lambda and Delta parameters for the current working environment and other parameters.

- Lambda and Delta are measured in pixels, but the number may not be an integer.
- Row Index: initial row number
- Row Arrangement: row arrangement (0 - vertical, 1 - horizontal)

## Status Line

![/imgstore/status_line.png](/imgstore/status_line.png)

The status bar displays the current state of the workspace environment:

- Selected: Shows the size of the selected area in absolute (pixels) and relative (Lambda) units. If pattern is selected - shows its name and position.
- Scroll: Shows the scrolling offset coordinates relative to the upper left corner of the source image
- Patterns: Indicates the number of patterns in the database that match the filter criteria (select box), and the total number of patterns in the database
- Patterns Added: The amount of added patterns
- Lambda/Delta: The current Lambda/Delta settings of working environment
- Source Image: The name of the original image

## Row number display

The row numbers are also displayed. The starting row number and their arrangement (vertical/horizontal) can be set in the settings.

|Vertical rows|Horizontal rows|
|---|---|
|![/imgstore/vert_rows.png](/imgstore/vert_rows.png)|![/imgstore/horz_rows.png](/imgstore/horz_rows.png)|
