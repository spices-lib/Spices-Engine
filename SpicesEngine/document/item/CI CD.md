CI/CD
============
# CI
***
* SpicesEngine Solution uses Github Action for CI
  * [Linking](https://github.com/spices-lib/Spices-Engine/actions)
  * ScreenShot:
  <img alt="CI" src="C:/Users/spiecs/Pictures/Screenshots/CI.png"/>
* CI workflows
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/.github/workflows/CI.yml)
  * ScreenShot:
  <img alt="CI" src="C:/Users/spiecs/Pictures/Screenshots/CI workflows.png"/>
  * Steps:
    1. clone the repository recursive.
    2. run
      ~~~
      ./Startup.bat
      ~~~
    3. run
      ~~~
      ./Scripts/Build_Debug_x64.bat
      ~~~

# CD
***
* SpicesEngine Solution uses Vercel for CD
  * ScreenShot:
    <img alt="CD" src="C:/Users/spiecs/Pictures/Screenshots/CD.png"/>
* vercel config
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/vercel.json)
  * ScreenShot:
  <img alt="vercel" src="C:/Users/spiecs/Pictures/Screenshots/vercel.png"/>
* package config
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/package.json)
  * ScreenShot:
    <img alt="package" src="C:/Users/spiecs/Pictures/Screenshots/package.png"/>
  * Steps:
    1. clone the repository recursive.
    2. npm run vercel-build.
    3. run 
      ~~~
      GenerateDocuments.sh
      ~~~
    4. Publish SpicesEngine Solution documents.