# Purpose

This file is to keep track of where I am in `snaptoken`'s 184 steps of
implementing the kilo editor.

- [x] - [Table of contents](https://viewsourcecode.org/snaptoken/kilo/index.html)

## Chapter 1: - [Setup](https://viewsourcecode.org/snaptoken/kilo/01.setup.html)

- [x] Step 1: Implement  an empty `main` function.
- [x] Step 2: Create a `Makefile`.

## Chapter 2: - [Entering raw mode](https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html)

- [x] Step 3:
- [x] Step 4: Press `q` to quit?
- [x] Step 5: Turn off echoing
- [x] Step 6: Disable raw mode at exit
- [x] Step 7: Turn off canonical mode
- [x] Step 8: Display kepresses
- [x] Step 9: Turn off `Ctrl-C` and `Ctrl-Z` signals
- [x] Step 10: Disable `Ctrl-S` and `Ctrl-Q`
- [x] Step 11: Disable `Ctrl-V`
- [x] Step 12: Fix `Ctrl-M`
- [x] Step 13: Turn off all output processing
- [x] Step 14:
- [x] Step 15: Miscellaneous flags
- [x] Step 16: A timeout for `read()`
- [x] Step 17: Error handling
- [x] Step 18:
- [x] Step 19: Sections

## Chapter 3: - [Raw input and output](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html)

- [x] Step 20: [Press `Ctrl-Q` to quit](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#press-ctrl-q-to-quit)
- [x] Step 21: [Refactor keyboard input](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#refactor-keyboard-input)
- [x] Step 22: [Clear the screen](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#clear-the-screen)
- [x] Step 23: [Reposition the cursor](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#reposition-the-cursor)
- [x] Step 24: [Clear the screen on exit](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#clear-the-screen-on-exit)
- [x] Step 25: [Tildes](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#tildes)
- [x] Step 26: [Global state](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#global-state)
- [x] Step 27: [Window size, the easy way](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#window-size-the-easy-way) - use `ioctl(TIOCGWINSZ)`
- [x] Step 28: Window size, the easy way - storing window size
- [x] Step 29: Window size, the easy way - print the proper number of tildes
- [x] Step 30: [Window size, the hard way](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#window-size-the-hard-way) - moving the cursor to the bottom-right
- [x] Step 31: Window size, the hard way - get the cursor position
- [x] Step 32: Window size, the hard way - reading the cursor position
- [x] Step 33: Window size, the hard way - converting the response to integers
- [x] Step 34: Window size, the hard way - restore control flow
- [x] Step 35: [The last line](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#the-last-line) - skipping the line terminator
- [x] Step 36: [Append buffer](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#append-buffer) - struct definition
- [x] Step 37: Append buffer - `abAppend` and `abFree` functions
- [x] Step 38: Append buffer - adding to existing code
- [x] Step 39: [Hide the cursor when repainting](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#hide-the-cursor-when-repainting)
- [x] Step 40: [Clear lines one at a time](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#clear-lines-one-at-a-time)
- [x] Step 41: [Writing welcome message](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#welcome-message)
- [x] Step 42: Centering welcome message
- [x] Step 43: [Move the cursor](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#move-the-cursor) - Define `cx` and `cy`, initialize to 0, 0
- [x] Step 44: Move the cursor - Set the cursor position to `cx`, `cy`
- [x] Step 45: Move the cursor - adws keys
- [x] Step 46: [Arrow keys](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#arrow-keys)
- [x] Step 47: Arrow keys - using an enumeration
- [x] Step 48: Arrow keys - giving enumeration non-character values
- [x] Step 49: [Prevent moving the cursor off screen](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#prevent-moving-the-cursor-off-screen)
- [x] Step 50: [The `Page Up` and `Page Down` keys](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#the-page-up-and-page-down-keys) - read key sequences
- [x] Step 51: The `Page Up` and `Page Down` keys - process keys
- [x] Step 52: [The `Home` and `End` keys](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#the-home-and-end-keys) - read key sequences
- [x] Step 53: The `Home` and `End` keys - process keys
- [x] Step 54: [The `Delete` key](https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#the-delete-key)

## Chapter 4: - [A text viewer](https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html)

- [x] Step 55: [A line viewer](https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html#a-line-viewer) - declarations and initialization
- [x] Step 56: A line viewer - hard coded "hello world" string
- [x] Step 57: A line viewer - displaying the string
- [x] Step 58: A line viewer - reading a line from a file
- [x] Step 59: A line viewer - feature test macros
- [x] Step 60: A line viewer - only display the welcome message when there's no file
- [x] Step 61: [Multiple lines](https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html#multiple-lines) - making file lines an array, initializing
- [x] Step 62: Multiple lines - appending a line in a function
- [x] Step 63: Multiple lines - appending lines to an array
- [x] Step 64: Multiple lines - fetching indext lines in <tt>editor_draw_rows</tt>
- [x] Step 65: Multiple lines - looping to read a while file
- [x] Step 66: [Vertical scrolling](https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html#vertical-scrolling) - declare and initialize <tt>line_offset</tt>
- [x] Step 67: Vertical scrolling - get offset line in <tt>editor_draw_rows</tt>
- [x] Step 68: Vertical scrolling - add the <tt>editor_scroll</tt> function
- [x] Step 69: Vertical scrolling - allow the cursor to scroll past the bottom of the screen
- [x] Step 70: Vertical scrolling - position the cursor correctly based on new interpretation of cy
- [ ] Step 71: Horizontal scrolling
- [ ] Step 72: 
- [ ] Step 73: 
- [ ] Step 74: 
- [ ] Step 75: 
- [ ] Step 76: Limit scrolling to the right
- [ ] Step 77: Snap cursor to end of line
- [ ] Step 78: Moving left at the start of a line
- [ ] Step 79: Moving right at the end of a line
- [ ] Step 80: Rendering tabs
- [ ] Step 81: 
- [ ] Step 82: 
- [ ] Step 83: 
- [ ] Step 84: 
- [ ] Step 85: Tabs and the cursor
- [ ] Step 86: 
- [ ] Step 87: 
- [ ] Step 88: 
- [ ] Step 89: 
- [ ] Step 90: Scrolling with `Page Up` and `Page Down`
- [ ] Step 91: Moving to the end of the line with `End`
- [ ] Step 92: Status bar
- [ ] Step 93: 
- [ ] Step 94: 
- [ ] Step 95: 
- [ ] Step 96: 
- [ ] Step 97: Status Message
- [ ] Step 98: 
- [ ] Step 99: 
- [ ] Step 100: 

## Chapter 5: - [A text editor](https://viewsourcecode.org/snaptoken/kilo/05.aTextEditor.html)

- [ ] Step 101: Insert ordinary characters
- [ ] Step 102: 
- [ ] Step 103: 
- [ ] Step 104: Prevent inserting special characters
- [ ] Step 105: Save to disk
- [ ] Step 106: 
- [ ] Step 107: 
- [ ] Step 108: 
- [ ] Step 109: 
- [ ] Step 110: 
- [ ] Step 111: Dirty flag
- [ ] Step 112: 
- [ ] Step 113: 
- [ ] Step 114: 
- [ ] Step 115: Quit confirmation
- [ ] Step 116: Simple backspacing
- [ ] Step 117: 
- [ ] Step 118: 
- [ ] Step 119: Backspacing at the start of a line
- [ ] Step 120: 
- [ ] Step 121: 
- [ ] Step 122: The `Enter` key
- [ ] Step 123: 
- [ ] Step 124: 
- [ ] Step 125: 
- [ ] Step 126: Save as...
- [ ] Step 127: 
- [ ] Step 128: 
- [ ] Step 129: 
- [ ] Step 130: 

## Chapter 6: - [Search](https://viewsourcecode.org/snaptoken/kilo/06.search.html)

- [ ] Step 131: Search
- [ ] Step 132: 
- [ ] Step 133: 
- [ ] Step 134: 
- [ ] Step 135: Incremental Search
- [ ] Step 136: 
- [ ] Step 137: 
- [ ] Step 138: Restore cursor position when cancelling search
- [ ] Step 139: Search forward and backward
- [ ] Step 140: 
- [ ] Step 141: 

## Chapter 7: - [Syntax highlighting](https://viewsourcecode.org/snaptoken/kilo/07.syntaxHighlighting.html)

- [ ] Step 142: Colorful digits
- [ ] Step 143: Refactor syntax highlighting
- [ ] Step 144: 
- [ ] Step 145: 
- [ ] Step 146: 
- [ ] Step 147: 
- [ ] Step 148: 
- [ ] Step 149: 
- [ ] Step 150: Colorful search results
- [ ] Step 151: 
- [ ] Step 152: Restore syntax highlighting after search
- [ ] Step 153: Colorful numbers
- [ ] Step 154: 
- [ ] Step 155: 
- [ ] Step 156: 
- [ ] Step 157: Detect filetype
- [ ] Step 158: 
- [ ] Step 159: 
- [ ] Step 160: 
- [ ] Step 161: 
- [ ] Step 162: 
- [ ] Step 163: 
- [ ] Step 164: 
- [ ] Step 165: Colorful strings
- [ ] Step 166: 
- [ ] Step 167: 
- [ ] Step 168: 
- [ ] Step 169: Colorful single-line comments
- [ ] Step 170: 
- [ ] Step 171: 
- [ ] Step 172: Colorful keywords
- [ ] Step 173: 
- [ ] Step 174: 
- [ ] Step 175: Nonprintable characters
- [ ] Step 176: 
- [ ] Step 177: Colorful multiline comments
- [ ] Step 178: 
- [ ] Step 179: 
- [ ] Step 180: 
- [ ] Step 181: 
- [ ] Step 182: 
- [ ] Step 183: 
- [ ] Step 184: 
