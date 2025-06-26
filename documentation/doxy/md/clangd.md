
# Clangd configuration

  Clangd must be configured to never insert header files. This is to prevent conflicting and unnecessary inclusions,
  particularly with vulkan headers; since volk supplies what we require and is incompatible without the VK_NO_PROTOTYPES definition.

  Clangd should also be permitted to check for clang-tidy conformance while developing, this will drastically reduce time resolving this errors later.

  Be sure to use a modern version of clangd, at minimum version~18.0, to be sure everything is operating correctly.

  Due to issues with getting a .clangd configuration file to properly configure clangd, the current recommendation is to allow clangd to default for everything
  except the header-insertion. You should clarify to clangd that header-insertion is to NEVER occur under any circumstances during Venus development, see above for reasoning.

  If you are using vscode then you can install the clangd extension from llvm@org and then configure clangd arguments within your settings.
  You should not use quotations in the settings menu argument table.
  
  To disable header-insertion you must:

  ```bash
    --header-insertion=never
  ```
  
  This should automatically create a new row in your settings.json file:
  ```json
    "clangd.arguments": ["--header-insertion=never"]
  ```
