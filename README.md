# Mini Studio

Hello,

To start the project, please follow those steps :

-> Clone the repository

-> Open the folder via your IDE

-> Use your IDE's terminal and write ".\Bootstrap.bat"

-> Then click on File -> Open -> Solution and go to your folder, into the build folder then launch the .sln

-> Set "mainFile" as startup project

-> And go to Properties, Linker, Input and make sure you have those lines (for Debug Config) or it won't work :
    
    sfml-graphics-d.lib
     
    sfml-window-d.lib
     
    sfml-system-d.lib
    
    sfml-audio-d.lib

-> Enjoy !
