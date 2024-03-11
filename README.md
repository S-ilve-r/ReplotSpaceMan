# ReplotSpaceMan
Simple tool to only delete old plots when, and only when, space is needed for new files.  Intended for use by Chia famers during replotting tasks where they inevitably want to retain as many of their old plots as possible and not delete them any sooner than they need to in order to make way for a new plot. This way, their farm size is always maximised.

Run the .exe and it will create it's own .config file on first run.

The intention is that you populate a list of folders to monitor, one for each drive. Once you hit the 'GO!' button, the app will monitor the free space of those drives and delete a single .plot file in the specified location each time that the available free space drops below the value in the .ini setting 'New Plot Size (GB)'.  This setting defaults to 75 GB.

Using a separate plotting tool (e.g. Bladebit CUDA) and it's built in load-balancing feature (at the time of writing this is configured using -d /path -d /path -d /path for BladeBit CUDA) to distribute plots to their farming locations and use ReplotSpaceMan to make space if and when it's needed.

*Known Issues/Limitations*
Hard limited to (now!) fifteen locations.
Initial check timer is set to 5 seconds which is quite short. Can be easily changed in the ReplotSpaceMan.ini file though.
Couldn't figure out how to get the (undocumented?) browse button in the 'MFC VSListBox Control' working so I disabled it so user has to enter paths manually.
Was thrown together in a couple of hours and intended for personal use only so it's not thoroughly tested. It has proven completely stable though and uses < 1mb of RAM.
Not tested on anything other than Win 11.
