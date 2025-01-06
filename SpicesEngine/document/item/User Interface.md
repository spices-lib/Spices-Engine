User Interface
============
* User Interface in SpicesEngine Solution is designed based on [Nvidia USD Composer](https://docs.omniverse.nvidia.com/composer/latest/index.html) style.
* This solution uses [ImGui](https://github.com/ocornut/imgui) as the UI library.

# MainMenu
***
## File
***
* **Entry:**
  ~~~
  MainMenu -> File
  ~~~
* **ScreenShot:**<br/>
  <img alt="File" src="File.png"/>

## Edit
***
* **Entry:**
  ~~~
  MainMenu -> Edit
  ~~~
* **ScreenShot:**<br/>
  <img alt="Edit" src="Edit.png"/>
### Preferences
* **Entry:**
  ~~~
  MainMenu -> Edit -> Preferences
  ~~~
* **ScreenShot:**<br/>
  <img alt="Preferences" src="Preferences.png"/>
#### Display Options
* **Entry:**
  ~~~
  MainMenu -> Edit -> Preferences -> Display Options
  ~~~
* **ScreenShot:**<br/>
  <img alt="Display Options" src="Display Options.png"/>
* **Grid:**
  * [Deserialize from material](https://github.com/spices-lib/Spices-Engine/blob/Master/SpicesEngine/assets/Materials/src/Material.ViewportGridRenderer.ViewportGrid.Default.material)
#### Post Processing
* **Entry:**
  ~~~
  MainMenu -> Edit -> Preferences -> Post Processing
  ~~~
* **ScreenShot:**<br/>
  <img alt="Post Processing" src="Post Processing.png"/>
* **Bloom:**
  * [Deserialize from material](https://github.com/spices-lib/Spices-Engine/blob/Master/SpicesEngine/assets/Materials/src/Material.PostProcessRenderer.Bloom.Default.material)
* **ToneMapping:**
  * [Deserialize from material](https://github.com/spices-lib/Spices-Engine/blob/Master/SpicesEngine/assets/Materials/src/Material.PostProcessRenderer.ToneMapping.Default.material)

## Create
***
* **Entry:**
  ~~~
  MainMenu -> Create
  ~~~
* **ScreenShot:**<br/>
  <img alt="Create" src="Create.png"/>
### Mesh
* **Entry:**
  ~~~
  MainMenu -> Create -> Mesh
  ~~~
* **Cone:**
* **Cube:** Create an Entity with CubePack MeshComponent Async.
* **Cylinder:**
* **Disk:**
* **Plane:** Create an Entity with PlanePack MeshComponent Async.
* **Sphere:** Create an Entity with SpherePack MeshComponent Async.
* **Tours:**

## Window
***
* **Entry:**
  ~~~
  MainMenu -> Window
  ~~~
* **ScreenShot:**<br/>
  <img alt="Window" src="Window.png"/>

## Tools
***
* **Entry:**
  ~~~
  MainMenu -> Tools
  ~~~
* **ScreenShot:**<br/>
  <img alt="Tools" src="Tools.png"/>

## Layout
***
* **Entry:**
  ~~~
  MainMenu -> Layout
  ~~~
* **ScreenShot:**<br/>
  <img alt="Layout" src="Layout.png"/>
* **[Default](https://github.com/spices-lib/Spices-Engine/blob/Master/SpicesEngine/assets/SlateLayout/DefaultLayout.ini):** Switch current UI Layout to Default style.
* **Animation:**
* **Animation Graph:**
* **Paint:**
* **Rendering:**
* **Visual Scripting:**
* **Physics Authoring:**
* **UI Toggle Visibility:** Toggle the selected Viewport.
* **Fullscreen Mode:** Toggle the selected project.
* **Save Layout:** Save the current UI Layout to file.
* **Load Layout:** Load a UI Layout from file.
* **Quick Save:** Save the current UI Layout to cache quickly.
* **Quick Load:** Load a UI Layout from cache quickly.

## Help
***
* **Entry:**
  ~~~
  MainMenu -> Help
  ~~~
* **ScreenShot:**<br/>
  <img alt="Help" src="Help.png"/>

# Viewport
***
## Viewport
***
## FloatingInfo
***
## Gizmos
***
## ViewportToolBar

# Console
***

# ContentBrowser
***

# InfoBar
***

# MaterialPanel
***

# Operation
***

# Stage
***