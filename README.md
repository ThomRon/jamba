Jamba - A lightweight VST2/3 framework
======================================

Jamba is a set of helpers (classes, concepts, build files, etc...) built on top of the VST SDK to provide a lightweight framework to build a VST2/3 plugin. Jamba has been designed to help in building VST2/3 plugin, not to replace it: you are still writing a VST2/3 plugin, not a Jamba plugin.

Features
--------

- generate a fully buildable, testable, editable and deployable plugin with a simple command (see [Quick Starting Guide](#quick-starting-guide))
- build a self contained plugin that depends on the VST3 SDK
- optionally builds a VST2 compatible plugin
- optionally wraps the VST3 plugin into an Audio Unit (macOS)
- easily write unit tests for your project (using googletest)
- build on macOS and Windows 10
- package your project as an archive (zip file) for production release
- run a simple script to build, test, validate, edit and install the plugin from the command line
- pick and choose which feature is useful to you (lots of options to override/change the default behavior)
- define your VST parameters in one location (inherit from `Parameters`) and use both in real time processing (RT) and GUI code
- define typed VST parameters (ex: a boolean (`bool`) parameter, a gain (`Gain`) parameter, etc...)
- use typed VST parameters directly in RT processing code as well as GUI code
- use Jamba parameters for handling non VST parameters (ex: a label/text string cannot be represented by a VST parameters). Jamba parameters can be automatically saved/restored part of the state
- use Jamba parameters to easily exchange messages between RT and GUI (both directions) in a thread safe fashion
- easily create custom views with their creators (so that they appear in the GUI editor)
- easily use multiple parameters in a custom view (ex: a custom display which displays a gain value (parameter 1) in a color depending on parameter 2)
- store/read state in a thread safe fashion (includes version)
- included views:
  - Toggle button and Momentary button with 2 or 4 frames
  - Text Edit (with input saved part of the state)
  - Scrollbar (with optional zoom handles)
  - Text button (to handle clicks with listener)
  - Step Button (to increment/decrement a param value by a fixed amount)
  - Discrete Button ("on" only when discrete param value matches (ex: radio buttons, tabs, etc...))
  - Switch Container (to switch between multiple views based on (discrete) param value (ex: tabs))

Quick starting guide
--------------------

This project is known to work on macOS High Sierra 10.13.3 with Xcode 9.2 installed. It also has been tested on Windows 10 64 bits and Visual Studio Build tools (2017). It requires `cmake` version 3.12 at the minimum. Linux is currently not supported (could be added if there is demand).

### Step 1. Install the VST3 SDK

#### Download the SDK
You need to download the [VST3 SDK version 3.6.9](https://github.com/pongasoft/vst3sdk/releases/download/vst369_01_03_2018_build132/vstsdk369_01_03_2018_build_132.zip) (shasum 256 => `7c6c2a5f0bcbf8a7a0d6a42b782f0d3c00ec8eafa4226bbf2f5554e8cd764964`). Note that 3.6.10 was released in June 2018 but at this time, this project uses 3.6.9.

#### Install the SDK

Unpack the SDK to the following location (note how it should be renamed with the version number)

* `/Users/Shared/Steinberg/VST_SDK.369` for macOS
* `C:\Users\Public\Documents\Steinberg\VST_SDK.369` for windows.

You can also store in a different location and use the `VST3_SDK_ROOT` variable when using cmake to define its location.

#### Configure the SDK (for VST2)

In order to build both VST2 and VST3 at the same time, you need to run the following commands

    # for macOS
    cd /Users/Shared/Steinberg/VST_SDK.369
    ./copy_vst2_to_vst3_sdk.sh

    # for Windows
    cd C:\Users\Public\Documents\Steinberg\VST_SDK.369
    copy_vst2_to_vst3_sdk.bat

### Step 2. Clone this repository

Simply clone this repository in a location of your choice (referred to as JAMBA\_REPO\_DIR). This gives access to the blank plugin files and `create-plugin.py` script.

### Step 3. Install python3

The `create-plugin.py` script requires `python3` to run so you need to install it if you don't already have it (note that the latest version of macOS 10.13.6 still comes with python2). Note that Jamba is a C++ (14) framework and only uses python for the convenience of creating a blank plugin.

### Step 4. Generate a blank plugin

From a directory of your choice, run the following command:

    python3 <JAMBA_REPO_DIR>/create-plugin.py

Follow the prompt (example run)

    > cd /tmp
    > python3 /Volumes/Development/github/org.pongasoft/jamba/create-plugin.py
    Plugin Name (must be a valid C++ class name) = Kooza
    Enable VST2 (Y/n)?
    Enable Audio Unit (Y/n)?
    Audio Unit manufacturer code (4 chars / one capital letter) (leave empty for default [Kooz]) =
    Filename (leave empty for default [Kooza]) =
    Company (leave empty for default [acme]) = pongasoft
    Company URL (leave empty for default [https://www.pongasoft.com]) =
    Company Email (leave empty for default [support@pongasoft.com]) = 
    C++ namespace (leave empty for default [pongasoft::VST::Kooza]) = 
    Project directory (leave empty for default [/private/tmp]) = 
    Project Name (leave empty for default [pongasoft-Kooza-plugin]) = 
    ##################
    Plugin Name        - Kooza
    VST2 Enabled       - ON
    Audio Unit Enabled - ON
    AU Plugin Code     - Kooz
    Filename           - Kooza (will generate Kooza.vst3)
    Company            - pongasoft
    Company URL        - https://www.pongasoft.com
    Company Email      - support@pongasoft.com
    Jamba git hash     - v2.1.0
    C++ Namespace      - pongasoft::VST::Kooza
    Plugin root dir    - /private/tmp/pongasoft-Kooza-plugin

    Are you sure (Y/n)?
    Generating Kooza plugin....
    Kooza plugin generated under /private/tmp/pongasoft-Kooza-plugin
    You can now configure the plugin:

    For macOs:
    ----------
    ### configuring

    cd <build_folder>
    /private/tmp/pongasoft-Kooza-plugin/configure.sh

    ### building, testing, etc...
    cd <build_folder>/build
    ./jamba.sh -h

    For Windows 10:
    ---------------
    ### configuring

    cd <build_folder>
    /private/tmp/pongasoft-Kooza-plugin/configure.bat

    ### building, testing, etc... (PowerShell recommended)
    cd <build_folder>/build
    ./jamba.bat -h

The blank plugin generated is simply copying input to output.

### Step 5. Build and test the plugin

After generating the plugin, the script gives you instructions on the various commands that you can run:

- You create a build folder outside the generated plugin (or inside if you prefer since the `configure.sh(.bat)` command will create a `build` folder which is excluded in `.gitignore`).
- Then you run the `configure.sh(.bat)` step to generate the project (Xcode or Visual Studio) and main script.
- Then simply run `jamba.sh(.bat) -h` to get the list of all commands that you can run

         > ./jamba.sh -h
         Usage:  jamba.sh [-hdrn] <command>

           -h : usage help
           -d : use Debug build config (default)
           -r : use Release build config (Debug if not defined)
           -n : dry run (prints what it is going to do)

         Commands: 
           ---- VST Commands ----
           clean    : clean all builds
           build    : build the VST plugin
           edit     : start the GUI editor (Debug only)
           install  : install the VST plugin in their default location
           test     : run the unit tests
           validate : run the VST3 validator
           ---- Audio Unit Commands ----
           build-au   : build the Audio Unit wrapper plugin
           install-au : install the Audio Unit plugin in its default location
           validate-au : run the Audio Unit validator
           ---- Generic Commands ----
           archive : generate the zip file containing the plugin(s) and README/License
           prod    : run test/validate/archive (default to Release, override with -d)
           ---- CMake target ----
         <target> : invoke cmake with the provided target

Note: once the plugin is generated, feel free to edit/modify any file you want as the generating phase is only meant to be run once in order to quickly get a plugin with all the pieces (and boilerplate code) in place.

### Step 6. Add your own files

- If you add new source files, add them to the `CMakeLists.txt` file in the `vst_sources` section (feel free to change the `CMakeLists.txt` to your liking, for example if you want to include all files without having to explicitly add them this is possible... refer to `cmake` for more info).
- If you add new tests files, they are added automatically as long as they are located under `test/cpp`
- If you add new resources for the GUI (images for example), they need to be added to `CMakeLists.txt`:

        # example (see commented line in CMakeLists.txt)
        jamba_add_vst3_resource(${target} PNG "background.png")
        ...

    Note: In the UI editor when adding a bitmap and saving the `xxx.uidesc` file, the fullpath of the image might be recorded. Make sure to edit the `xxx.uidesc` and modify it to keep **ONLY** the filename
    
        <!-- this is WRONG!!!!! -->
        <bitmap name="background" path="/private/tmp/pongasoft-Kooza-plugin/resource/background.png"/>
        
        <!-- this is right -->
        <bitmap name="background" path="background.png"/>

Note: `cmake` automatically detects changes to `CMakeLists.txt` so you should not have to do anything special when you make any changes.
  
### Step 7. Next

Here are some recommended steps:

- If you have never written a VST3 plugin I would suggest reading the VST3 documentation that comes with the SDK (you can find more information on the concepts, how the editor works, etc...). At the end of the day you are still writing a VST3 plugin not a Jamba plugin! Simply open the `index.html` file at the root of the SDK.
- Check [jamba-sample-gain](https://github.com/pongasoft/jamba-sample-gain) for documentation and explanation of the Jamba concepts.
- In particular:
   - Check `<Name>.h` which is where you define the Vst (and Jmb) parameters your plugin will use
   - Check `RT/<Name>Processor.cpp` which contains the main logic of the plugin
   - Use the editor to change the UI of the plugin (which then saves the `resource/<Name>.uidesc` xml file). You can edit the xml file by hand, but it is definitely recommended to use the editor to do so.
   - Check `<Name>_VST2.cpp` on instructions about how to register the VST2 plugin ID with Steinberg (unfortunately this is a manual process). You don't have to do this right away... only when you are ready to release your plugin.

Documentation
-------------

- Check the [Latest doxygen documentation](http://pongasoft.github.io/jamba/docs/3.1.0/html/index.html)
- The [jamba-sample-gain](https://github.com/pongasoft/jamba-sample-gain) project was designed as a documentation plugin for this project.
- The [vst-vac-6v](https://github.com/pongasoft/vst-vac-6v) plugin is built with Jamba.
- The [vst-ab-switch](https://github.com/pongasoft/vst-ab-switch) plugin is built with Jamba.
- The [vst-sam-spl-64](https://github.com/pongasoft/vst-sam-spl-64) plugin is built with Jamba.
- The [Announcing Jamba](https://www.pongasoft.com/blog/yan/vst/2018/08/29/Announcing-Jamba/) blog post goes into more details about the history and the main features of the framework.

Release Notes
-------------
### 2019-05-19 - `v3.2.0`
* added audio unit validation to `jamba.sh` script (`jamba.sh validate-au`) which runs the `auvaltool` utility on the audio unit plugin
* added switch container view to handle tab like switching (check `Views::SwitchContainerView`)
* added (optional) image for text buttons (`Views::TextButtonView`)
* added (optional) disabled state for momentary buttons (`Views::MomentaryButtonView`)
* added discrete button view (a button which is "on" only when the backing discrete parameter matches the `step` value). This button can be used for radio groups, tabs, etc... (check `Views::DiscreteButtonView`)
* Note: There is a small potentially breaking API change with function `Views::createCustomView` (defined in `Views/CustomViewCreator.h`). Check [commit](https://github.com/pongasoft/jamba/commit/08d06ceda9b936b92c45b8bd9f22a93acfeb3995). The function takes an additional 2 parameters which can be safely ignored if you don't need them.

### 2019-02-10 - `v3.1.0`
* added ability to conveniently register a global key hook (check `Views::registerGlobalKeyboardHook`)
* parameters now copy the provided strings (title, short tile and units) so that they no longer have to be static and for example can be computed
* added convenient methods to `AudioBuffers` and `Channel` (`copyFrom` and `forEachSample`). Defined `const` and non `const` versions.
* added Step Button view which is a button that increment/decrement a parameter by a fixed amount when clicked. Another amount (tied to shift click) can be provided.

### 2019-01-06 - `v3.0.2`
* fixed minor bugs

### 2019-01-04 - `v3.0.1`
* Use shallow cloning to avoid downloading the (unused) project history

### 2019-01-03 - `v3.0.0`
* This is a major release with many changes (a few breaking APIs)
* Added `TextViewButton` on which you can register a `onClickListener` or implement `onClick`. Also handles disabled state.
* Added `ScrollbarView` which implements a scrollbar tied to 2 parameters (offset and zoom)
* Added `CustomController` to implement a custom controller tied into Jamba (access to Vst/Jmb parameters and state)
* Added ability to easily switch to a new view (`GUIController::switchToView`)
* Added `GUIJmbParameter::updateIf` to update the value in place when necessary
* Added callback APIs to `GUIParamCxAware`

        registerCallback<bool>(fParams->fMyParam, [this]  (GUIVstParam<bool> &iParam) { flushCache(); });

* Added registering callbacks and parameters on a view without inheriting from it (can be used from controllers `verifyView` method):

        auto button = dynamic_cast<Views::TextButtonView *>(iView);
        if(button) {
          auto callback = [] (Views::TextButtonView *iButton, GUIJmbParam<SampleData> &iParam) {
            iButton->setMouseEnabled(iParam->hasUndoHistory());
          };
          fState->registerConnectionFor(button)->registerCallback<SampleData>(fState->fSampleData, std::move(callback), true);
        }

* Added optional arguments to `Parameters::vst<>()` (resp. `Parameters::jmb<>()`) that get passed through the converter (resp. serializer) allowing syntax like

        fPlayModeHold =
          vst<BooleanParamConverter>(1201, // param ID
                                     STR16("Play Mode"), // param title
                                     STR16("Trigger"), STR16("Hold")) // BooleanParamConverter args

* Requires C++14
* Added `EnumParamConverter` for Vst parameters backed by an enum
* Added `Range` concept
* Refactored `CustomViewCreator` code to simplify writing individual attributes. Introduced `MarginAttribute`, `RangeAttribute`, and `GradientAttribute`

The following changes are potentially breaking changes:

* Refactored `Lerp` class to deal with type parameters differently (`TFloat` for math precision, `X` for type of x, `Y` for type of y). Introduced `SPLerp` (single precision) and `DPLerp` (double precision) as well as several convenient methods. Example:

        // this will interpolate (SP=single precision)
        // X -> the time (long) from the range [0, fFadeDuration]
        // Y -> to the alpha (uint8_t) range [255, 0] (opaque -> transparent)
        // Note that although X and Y types are integer flavors, the interpolation will use floats
        // and the value returned will be cast to uint8_t
        fColor.alpha = Utils::mapValueSPXY<long, uint8_t>(iTime, 0, fFadeDuration, 255, 0);

* `GUIParamCxAware` (which is the class used for registering parameters) has been simplified with `registerParam` methods (when the type is known).
* Moved `PluginAccessor` into its own header file
* Removed `CustomViewInitializer`


### 2018-10-11 - `v2.1.2`
* `AudioBuffers` now properly handles null buffers
* Fixed issue with non Xcode build (Xcode generator is only required for audio unit building) and allow building with CLion

### 2018-10-06 - `v2.1.1`
* Fixed #4: Generate AU Plugin Manufacturer code from name instead of company

### 2018-10-06 - `v2.1.0`
* Implemented #1: _Eliminate resource declaration duplication_
* Implemented #2: _Optionally generate audio unit wrapper_
* Implemented #3: _Make VST2 support truly optional_
* Upgraded to cmake 3.12
* Consolidated dev scripts into a single `jamba.sh` (resp `jamba.bat`) script
* Uses `Xcode` as the cmake generator on macOS

### 2018-09-02 - `v2.0.2`
* Updated license terms

### 2018-08-28 - `v2.0.1`
* Added ability to create a jamba enabled blank plugin: the plugin simply copies the input to the output and offers all the capabilities of jamba (build, test, edit, validate, install, etc...).

### 2018-08-25 - `v2.0.0`
* Introduced Jamba parameters to handle non VST parameters and messaging RT <-> GUI
* Added Debug::ParamTable and Debug::ParamLine to display parameters => example

        | ID   | TITLE      | TYP | OW | TRS | SHA | DEF.N | DEF.S          | STP | FLG   | SHORT  | PRE | UID | UNS |
        --------------------------------------------------------------------------------------------------------------
        | 1000 | Bypass     | vst | rt |     |     | 0.000 | Off            | 1   | 65537 | Bypass | 4   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 2010 | Left Gain  | vst | rt |     |     | 0.700 | +0.00dB        | 0   | 1     | GainL  | 2   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 2011 | Right Gain | vst | rt |     |     | 0.700 | +0.00dB        | 0   | 1     | GainR  | 2   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 2012 | Link       | vst | ui |     |     | 1.000 | On             | 1   | 1     | Link   | 4   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 2020 | Reset Max  | vst | rt |     |     | 0.000 | Off            | 1   | 1     | Reset  | 4   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 2000 | VuPPM      | vst | rt | x   |     | 0.000 | 0.0000         | 0   | 1     | VuPPM  | 4   | 0   |     |
        --------------------------------------------------------------------------------------------------------------
        | 3000 | Stats      | jmb | rt | x   | x   |       | -oo            |     |       |        |     |     |     |
        --------------------------------------------------------------------------------------------------------------
        | 2030 | Input Text | jmb | ui |     |     |       | Hello from GUI |     |       |        |     |     |     |
        --------------------------------------------------------------------------------------------------------------
        | 3010 | UIMessage  | jmb | ui | x   | x   |       |                |     |       |        |     |     |     |
        --------------------------------------------------------------------------------------------------------------

* Implemented lock free, memory allocation free and copy free version of SingleElementQueue and AtomicValue
* Generate build, test, validate, edit and install scripts
* Added [jamba-sample-gain](https://github.com/pongasoft/jamba-sample-gain) documentation project

### 2018-08-05 - `v1.0.0`
* first public release / free / open source

History
-------
This project was created to abstract the common patterns and solutions to many questions implemented across the VAC-6V and A/B Switch vst plugins. Check [Announcing Jamba](https://www.pongasoft.com/blog/yan/vst/2018/08/29/Announcing-Jamba/) blog post for more details.

Misc
----

- This project uses [loguru](https://github.com/emilk/loguru) for logging (included under `src/cpp/pongasoft/logging`)
- This project uses the VST3 SDK by Steinberg Media Technologies GmbH

Licensing
---------

- Apache 2.0 License. Jamba itself can be used according to the terms of the Apache 2.0 license.

- Note that the primary goal of Jamba is to help build a VST3 plugin and as a result you should check the [Steinberg License](http://www.steinberg.net/sdklicenses_vst3) in order to determine under which terms your plugin needs to be licensed.
