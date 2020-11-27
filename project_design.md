# VM Project

## Project Breakdown

### Timeline

| Stage              | Plan                                                         | Date   |
| ------------------ | :----------------------------------------------------------- | ------ |
| MVP                | - Skeletons of all classes, revaluate UML if needed.<br>- Open vm for a file and display contents.<br>- Cursor movement<br>- Write / delete characters<br>- Esc, :wq, and save file<br>- Test suite | Dec 1  |
| Core features      | - All text commands                                          | Dec 5  |
| More features      | - Undo<br>- Search<br>- Macros                               | Dec 10 |
| Bells and whistles | - Faster search<br>- Optimize undo feature for memory        | Dec 14 |

## UML

See `uml.pdf`

## Overview

*Your document should provide an overview of all aspects of your project, including how, at a high level, they were implemented. If you made use of design patterns, clearly indicate where. Your system should employ good object-oriented design techniques, as presented in class.*

The `App` class contains a `TextModel`, `Renderer`, and `Controller` to implement the model, view, controller paradigm. The `App` class contains the main run loop which functions as follows:

```c++
while(true) {
  renderer.display() // Render all views
  cmd = contoller.getCommand() // Parse input into command
  text_data.apply(cmd) // Visitor pattern
}
```

### Model

The `TextData` model contains the core state of the application. This includes the current state of the text, cursor position, and write / command mode. These properties are accessed through getter and setter methods by the `Renderer` and `Controller` classes. The model also contains basic commands such as `move(x,y)`, `insert(x,y)`, `delete(x,y)`which are used by subclasses of `Command` to implement more complex commands such as `dw` which moves the cursor to the beginning of the word and deletes that word.

### View

The `Renderer` implements the observer pattern where views can be subscribed and then rendered when the `render()` method it called. The `render()` method works by calling the `draw()` method of each individual view. Views pull data from the `TextModel` class and render it using the ncurses library.

### Controller

The `Keyboard` is the only `Controller` used by the application. The `Keyboard` parses the input character and depending on the state of the application, based on data in `TextModel`. For example an input would be parsed differently depending on wether the app is in command or insert mode. The `getCommand` method returns a `Command` object which is then applied to the `TextModel` in the main loop. This is implemented as an instance of the visitor pattern where a command would visit the text model class through the apply method and have access to `TextModels` getter/setter methods to perform the necessary modifications. These include changing both the text and cursor state.



## QA

### Support for multiple files

To support multiple files you could swap `text_model`'s. The the current implementation has a core model with a `text_model` for the file being operated on. This model contains information on the current state of the text inside the file as well as the state of the cursor and command / edit state. Opening a new file would be controlled by a "meta" menu where you could select a new file to open. The `text_model` model for the new file could be stored in a vector of `text_model` pointers in the main model. This would allow you to easily switch between files (preversing you currect state of work) using the meta menu which could take the form of command-[#number] like switching tabs in Chrome.



### Read only file

To add support for either of the options you would first have to check if a file is read only. We could accomplish this by trying to open a `std::ofstream` and checking if it successfully opened `ofs.is_open()`. You could add a property to the `TextModel` which keep track of the read permissions.

The simplest option would be to prevent any edits. The `getCommand()` method in `Controller` could check if the command is labelled as modifying the text, and simply ignore it if it does. Commands that don't modify the text could take a const reference to the `TextModel`. Any commands that don't take a const reference would be ignored. Alternatively you could have a lookup table for which commands modify the text.

To support edits allowed, but requiring to save to separate file, the views could use the read permissions property in the `TextModel` to display a warning that the file is read only. If the save method is called and `read_only` is asserted you would throw and error. You could still save if you specify a different filename (could check this in `FileOp` save method).



