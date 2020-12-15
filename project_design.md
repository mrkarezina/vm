# vm

## Project Breakdown

### Timeline

| Stage          | Plan                                                         | Date   |
| -------------- | :----------------------------------------------------------- | ------ |
| MVP            | - Skeletons of all classes, revaluate UML if needed.<br>- Open vm for a file and display contents.<br>- Cursor movement<br>- Write / delete characters<br>- Esc, :wq, and save file<br>- Test suite | Dec 1  |
| Basic features | - All text commands                                          | Dec 5  |
|                | - Undo<br>- Search<br>- Macros                               | Dec 10 |
| Extra Features | - Faster search<br>- Optimize undo feature for memory        | Dec 14 |

## Intro

Vm is like Vim but worse. If you're looking for a highly configurable text editor built to make creating and changing any kind of text very efficient, look elsewhere.

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

The model family only contains a single class, `TextModel`. The `TextModel` contains the core state of Vm. Some examples include current state of the text, cursor position, and write / command mode settings. These properties are accessed and mutated through getter and setter methods which power the the View and Command classes.

### View

Rendering is implemented using the `ncurses` library which allows for creating text based user interfaces. The views are stateless and solely responsible for displaying the state of the text model. Vm uses several windows to compose the entire view. These include the status bar at the bottom of the application and a view for the text being edited. The observer pattern is used to "subscribe" views to the `TextModel`. The `TextModel` stores the views as a vector of references to the `ViewBase` family of classes. The render loop calls the `draw` method of each view, as demonstrated in the implementation. 

```c++
void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
```

The `TextModel` object passes itself to each view via the `this` pointer. This allows the views to access methods such as `get_lines`, `get_x`, and  `get_y`. The data is then drawn using a thin wrapper around  the ncurses library.

### Command

Each command or family of commands is implemented as a class. These commands take a reference to the text model and use its getter and setter methods to perform the intended effect. This may include changing the text or the position of the cursor. The interaction between the `TextModel` and commands follows the visitor pattern. This is demonstrated by the `apply` method of the `TextModel`, where a command 'visits' the text model.

```c++
void TextModel::apply(unique_ptr<CmdBase> cmd) {
  cmd->exec(this);
}
```

The `TextModel` passes itself to the `exec` method of each command via the `this` pointer similar to the views. This allows each command to use the getter and setter methods of the text model to perform an edit on the data based on the current state of the text.

The `Controller ` parses all user input. Its job is the determine which command or series of commands correspond to the character the user entered. The corresponding command of type `CmdBase` is then generated.



## Design

*Techniques you used to solve the various design challenges in the project. Describe how your design supports the possibility of various changes to the program specification.*

This section explores the design challenges, design tradeoffs, and flexibility of the chosen design.

### Text Model

The function of the `TextModel` is complex since it stores the state of the entire application. This includes the text being edited, cursor position, and all temporary state persisted between commands. To make the design more maintainable and to make use of separate compilation standalone states such a `History` (undo command), `Clipboard` (yank and paste), and `Macros` (macro recordings) where split off into their own classes. The `TextModel ` native methods  are focused on editing the text data while other state that needs to be persisted between complex commands can be added as a class focusing on managing a specific type of state, for example a macro recording or yanked lines. This makes the design more maintainable since if a new complex family of commands were to be added, a corresponding class would be created to manage their state and it could be added as another property to the `TextModel`. This would prevent changes from being made to the `TextModel`'s  core methods.

### Views

The ncurses is a C library. A this wrapper, `Window` manages the pointer to the ncurses `WINDOW` object and deletes the window in the destructor. `Window` can then safely be used by the `ViewBase` family of classes which have a smart pointer to the `Window` object. This ensures windows are cleared and deleted when they go out of scope. Additionally, composing the views using Window's methods make the design more flexible if ncurses would need to be replaced. A wrapper could be implemented around a new library (ie: ncurses 2.0) with the same methods as `Window` and the views would function as needed.

Interactions between views and the model follow the observer pattern. Views are subscribed to the `TextModel` and stored in a vector of `ViewBase` objects. Any number of new views can be subscribed making the design flexible. An instance of `ViewBase` implemented the `draw()` method. All the subscribed views are rendered by the `render()` method, as described in the outline, by calling the `draw()` method of each view.

### Commands

The `Controller` is responsible for parsing the user input and returning the appropriate command object from `parse_input()`.

`CmdBase` is the base class for all commands. The uml depicts how all commands inherit from `CmdBase` and can form additional hierarchies of commands. The interactions between commands and the model follow the visitor pattern. The model have an apply method taking an object of type `CmdBase`, implementing an `exec(in Model*)` method. A command 'visits' the model through it's `apply(in CmdBase)` method which then calls the `exec` method of the command passing itself in via the this pointer. The command can then perform any edit on the model using it's getter and setter methods.

This design is flexible, allowing for new commands to be added without adding methods to the `TextModel`. Commands can implement any complex edit, such as moving to a new location and deleting, clearing copying some amount of text, using the basic getter and setter methods provided but the `TextModel`.

Instead of adding editing methods to the `TextModel` complex families of commands, such as motion, can inherit from a base class implementing shared methods, such as `CmdMoveBase` which in turn inherits from `CmdBase`. The UML depicts this family of commands. Motion commands such as `w`, `dw` may move the cursor and perform an edit. `CmdMoveBase` implements methods to find the end position after the movement for example `h` or `w`. An instance of  `CmdMoveBase`, such as `CmdMove` or `CmdyY`, implement the logic for what to do with the text between the start position and end position. In the case of `CmdMove` nothing happens, the cursor just moves. While `CmdyY` copies the text to the `Clipboard`.

Commands that can be broken down into composition of previous commands, such as `:wq` (write and quite), can be implemented as instances of `CmdMultiCommand`. This class stores a vector of `CmdBase` objects which are added using the `add_command()` method. `CmdSaveExit` implements `:wq` as a composition of the existing write and quite commands as follows:

```c++
CmdSaveExit::CmdSaveExit() {
  add_command(make_shared<CmdSaveLines>());
  add_command(make_shared<CmdQuit>());
}
```

Additionally `CmdMultiCommand` makes macros and multipliers delightfully easy to implement. Multipliers simply form a `CmdMultiCommand` containing n instances of the command being multiplied. Macros record commands as a vector of `CmdBase` object. Each command in this vector can then be added to form an instance of `CmdMultiCommand` which executes the series of recorded commands.

```c++
void CmdPlayBackMacro::exec(TextModel *model) {
  model->apply(model->macros->generate_playback_command(letter));
}
```

Grouping multiplers and macros into a `CmdMultiCommand` simplifies undo functionality as well since history is recorded after the application of a command to the model. Since a series of commands in `CmdMultiCommand` is recorded as a single command in history they can be undone easily.

## Bonus Features

Some small bonus features include:

- Multipliers on macros and nested macros
- Dot operator inside macros
- Up and down arrow keys
- Scroll percentage

The most notable of these are nested macros which might be an expectation already. This was possible to implemented since the series of commands corresponding to a macro generates a corresponding `CmdMultiCommand` object. Since every command inherits from `CmdBase`, a new macro may record a series of commands including new movements / edits with a previous macro (`CmdMultiCommand`) thrown into the mix.

## Questions

### How would you support multiple open files?

The constructor for `TextModel` takes a file name. An instance of `TextModel` stores all the information required to edit the file. This includes cursor position, lines of text, history and so on. To support multiple files a new higher level model would be created which would store a vector of `TextModels` similar to the model storing a vector of views, well call this `FileModel`.

There are two mistakes in the current implementation.The run loop and controller should be in the top level model instead of `TextModel`. Once this is fixed, by moving the run loop to the top level model, commands from the controller could be applied to `FileModel` instead of `TextModel` by swapping the `TextModel` with FileModel in the top level model. If the command switches files `FileModel` would simply switch the currently selected `TextModel`. Otherwise all other editing commands are forwarded and applied to `TextModel`.

### Support for read only files?

To add support for either of the options you would first have to check if a file is read only. We could accomplish this by trying to open a `std::ofstream` and checking if it successfully opened `ofs.is_open()`. You could add a property to the `TextModel` which stores the read permissions.

#### Option 1 (Preventing edits)

Commands that modify text can implement a `bool modifies_text()` method which returns true if they mutate the text. In the run loop after a command is parsed an if statement could be added to check if `modifies_text()` is true. In that case the command is not applied. This would require implementing  `modifies_text()` for every command.

#### Option 2 (Edits with warning)

The read permissions property in the `TextModel` would be used to display a warning that the file is read only. If the write command is called without a new filename and `read_only` is asserted you would play a beep sound. If the write command is called with a new file saving would work like usual. This would require implementing a single if statement in `CmdSaveLines`.

### What would you have done differently if you had the chance to start over?

1. Move the main run loop and controller to a top level model instead of keeping them in `TextModel`. This would make it trivial to support higher level functionality like multiple tabs / support for multiple files as described in the questions.
2. Decorators for command meta information. It would be helpful to have a decorator to set properties such as `modifies_text` or `dont_record_in_history`. This would make it easier to implement changes such as preventing edits, or implementing undo which groups certain commands together.
3. Structure parsing to be less brittle. Currently the order of the if statements in the parsing functions matters which is very error prone to accidentally adding a new command which will prevent a previously matching command from being matched accidentally.
4. For the MVP I had simple parsing function that would just match characters, however as the parsing logic grew more complex I continued adding if statements for each new command without having an idea how the overall parsing would work. It was challenging to refactor the parsing logic into functions parsing families of commands since the initial giant parsing function was brittle and the logic for what will be matched was hard to interpret. It still is and this is the weakest point of the project, I would give parsing more though had I started over.



