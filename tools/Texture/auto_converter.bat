@echo off
if not exist dds mkdir dds

for %%f in (*.tga) do (
    pushd %~dp0
    texconv.exe %%f -o dds
    popd
)
