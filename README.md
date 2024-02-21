# SimpleInjector

SimpleInjector is a simple command line tool to inject a DLL into a process using CreateRemoteThread. There's not much else here, I've just found I've basically written this code at least 10x and am tired of re-doing it constantly.

*This is a simple tool that is not meant to be used for anything other than educational purposes.*

## Usage

`./SimpleInjector.exe <process_name> <dll>`

Where `<process_name>` is the name of the process to inject into (e.g. `notepad.exe`) and `<dll>` is the path to the DLL to inject (e.g. `C:\Path\To\Dll.dll`).
