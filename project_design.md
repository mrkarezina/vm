# vm (vim ± 15%)

## Project Breakdown

### Timeline

| Stage          | Plan                                                         | Date   |
| -------------- | :----------------------------------------------------------- | ------ |
| MVP            | - Skeletons of all classes, revaluate UML if needed.<br>- Open vm for a file and display contents.<br>- Cursor movement<br>- Write / delete characters<br>- Esc, :wq, and save file<br>- Test suite | Dec 1  |
| Basic features | - All text commands                                          | Dec 5  |
|                | - Undo<br>- Search<br>- Macros                               | Dec 10 |
| Extra Features | - Faster search<br>- Optimize undo feature for memory        | Dec 14 |

## UML

See `uml.pdf`



## Intro

Vm is like Vim ± 15%, but worse. If you're looking for a highly configurable text editor built to make creating and changing any kind of text very efficient, look elsewhere. It is included as "vm" in no UNIX systems and definitely not with Apple OS X.

Vm is unstable and is occasionally being developed to become even worse. Among its limited features are:

- Editing commands
- Motion commands such as `c[any motion]`,` d[any motion]`, and `y[any motion]`
- Undo
- Search and replace
- Macros



## Overview

*Your document should provide an overview of all aspects of your project, including how, at a high level, they were implemented. If you made use of design patterns, clearly indicate where. Your system should employ good object-oriented design techniques, as presented in class.*

Vm was designed with the model view controller paradigm in mind has 3 main families of classes. Models, views, and commands.

At the highest level the views render the data stored in the text model. The controller parses the input to generate a command which is then applied to the text model. This process is demonstrated by the `render` method of the `TextModel`.

```c++
void TextModel::run() {
  while (render_loop_on) {
    render();
    apply(controller->parse_input());
  }
}
```

### Model

The model family only contains a single class, `TextModel`. The `TextModel` contains the core state of `vm`. This includes the current state of the text, cursor position, and write / command mode. These properties are accessed and mutated through getter and setter methods by the View and Command classes.

### View

Rendering is powered by the `ncurses` library which allows for creating text based user interfaces. The views are solely responsible for display the state of the text model. Vm uses several windows to compose the entire view. These include the status bar at the bottom of the application and a view for the text being edited. The observer pattern is used to "subscribe" views to the `TextModel`. The `TextModel` stores the views as a vector of references to the `ViewBase` family of classes. The render loop calls the `draw` method of each view, as demonstrated in the implementation. 

```c++
void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
```

The `TextModel` object is passed itself to each view via the `this` pointer. This allows the views to access methods such as `get_lines`, `get_x`, and  `get_y`. The data is then drawn using a thin wrapper around  the ncurses library.

### Command

Each command or family of commands is implemented as a class. These commands take a reference to the text model and use its getter and setter methods to perform the intended effect. This may include changing the text or the position of the cursor. In interaction between the `TextModel` and commands follows the visitor pattern. This is demonstrated by the `apply` method of the `TextModel`.

```c++
void TextModel::apply(unique_ptr<CmdBase> cmd) {
  cmd->exec(this);
}
```

The `TextModel` passes itself to the `exec` method of each command via the `this` pointer similar to the views. This allows each command to use the getter and setter methods of the text model to perform an edit on the data based on the current state of the text.

The `Controller ` parses all user input. Its job is the determine which command or series of commands correspond to the character the user entered.



## Design

*Techniques you used to solve the various design challenges in the project. Describe how your design supports the possibility of various changes to the program specification.*

This section explores the design challenges, design tradeoffs, and flexibility of the chosen design.

### Text Model

The function of the `TextModel` is complex since it stores the state of the entire application. This includes the text being edited, cursor position, and all temporary state persisted between commands. 



The model contains numerous method to change the state of the text. For example `delete_line` or `write_char`.



### Views



The observer pattern where views can be subscribed and then rendered when the `render()` method it called. The `render()` method works by calling the `draw()` method of each individual view. Views pull data from the `TextModel` class and render it using the ncurses library.

### Controller

The `Keyboard` is the only `Controller` used by the application. The `Keyboard` parses the input character and depending on the state of the application, based on data in `TextModel`. For example an input would be parsed differently depending on wether the app is in command or insert mode. The `getCommand` method returns a `Command` object which is then applied to the `TextModel` in the main loop. This is implemented as an instance of the visitor pattern where a command would visit the text model class through the apply method and have access to `TextModels` getter/setter methods to perform the necessary modifications. These include changing both the text and cursor state.







## Bonus Features







## Questions

### Support for multiple files

To support multiple files you could swap `text_model`'s. The the current implementation has a core model with a `text_model` for the file being operated on. This model contains information on the current state of the text inside the file as well as the state of the cursor and command / edit state. Opening a new file would be controlled by a "meta" menu where you could select a new file to open. The `text_model` model for the new file could be stored in a vector of `text_model` pointers in the main model. This would allow you to easily switch between files (preversing you currect state of work) using the meta menu which could take the form of command-[#number] like switching tabs in Chrome.



### Read only file

To add support for either of the options you would first have to check if a file is read only. We could accomplish this by trying to open a `std::ofstream` and checking if it successfully opened `ofs.is_open()`. You could add a property to the `TextModel` which keep track of the read permissions.

The simplest option would be to prevent any edits. The `getCommand()` method in `Controller` could check if the command is labelled as modifying the text, and simply ignore it if it does. Commands that don't modify the text could take a const reference to the `TextModel`. Any commands that don't take a const reference would be ignored. Alternatively you could have a lookup table for which commands modify the text.

To support edits allowed, but requiring to save to separate file, the views could use the read permissions property in the `TextModel` to display a warning that the file is read only. If the save method is called and `read_only` is asserted you would throw and error. You could still save if you specify a different filename (could check this in `FileOp` save method).



### What would you have done differently if you had the chance to start over?





