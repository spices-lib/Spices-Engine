CI/CD
============
# CI
***
* SpicesEngine Solution uses Github Action for CI
  * [Linking](https://github.com/spices-lib/Spices-Engine/actions)
  * ScreenShot:<br/>
    <img alt="CI" height="754" src="CI.png" width="1428"/>
* CI workflows
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/.github/workflows/CI.yml)
  * ScreenShot:<br/>
    <img alt="CI workflows" height="1077" src="CI workflows.png" width="1428"/>
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
* TODO
  1. add release build  to CD.
  2. add test steps to CD.

# CD
***
* SpicesEngine Solution uses Vercel for documents CD
  * ScreenShot:<br/>
    <img alt="CD" height="729" src="CD.png" width="1428"/>
* vercel config
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/vercel.json)
  * ScreenShot:<br/>
    <img alt="vercel" height="422" src="vercel.png" width="1424"/>
* package config
  * [Linking](https://github.com/spices-lib/Spices-Engine/blob/Master/package.json)
  * ScreenShot:<br/>
    <img alt="package" height="314" src="package.png" width="1428"/>
  * Steps:
    1. clone the repository recursive.
    2. npm run vercel-build.
    3. run 
      ~~~
      GenerateDocuments.sh
      ~~~
    4. Publish SpicesEngine Solution documents.
* TODO
  1. add binary pack to github release.