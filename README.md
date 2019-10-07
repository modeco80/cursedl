## cursedl
A Curse Minecraft modpack downloader. Made because everyone else's didn't work.

## How to build
- Clone the sources using `git clone --recursive https://github.com/modeco80/cursedl` (optionally add `--jobs <CPU CORE COUNT>` for faster clone)
- make a build directory
- cd into build directory
- `cmake ..`
- make (`-j <CPU CORE COUNT>`)
- ...
- Profit!

## How to install
`make install`

## Why does cursedl.cc use WriteFunction() instead of `cursedl::ui::WriteOutput()`?
This was an intentional choice to make sure the UI was seperated from the downloading logic.

This choice makes it easier for other projects to integrate the downloading logic into their projects.