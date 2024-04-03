# ReplotSpaceMan
Simple tool to only delete old plots as the space is needed for new files.  Intended for use by Chia famers during replotting tasks where they inevitably want to retain as many of their old plots as possible and not delete them any sooner than they need to in order to make way for a new plot.

Run the .exe and it will create it's own .config file on first run.

The idea is that you populate a list of folders to monitor, one for each drive. Once you hit the 'GO!' button, the app will monitor the free space of those drives and delete a single file in the specified location each time that the available free space drops below the value in the .ini setting 'New Plot Size (GB)'.  This setting defaults to 99 GB.

Using a separate plotting tool (e.g. Bladebit CUDA), and a separate file transfer utility (e.g. https://chiaforum.com/t/pschiaplotter-madmax-que-setup/10292), you feed new plots into a target drive and use ReplotSpaceMan to make space if and when it's needed. I use GigaHorse and it's load balancing final destination list makes replotting up to fifteen drives a set and forget process which is great.

*Known Issues/Limitations*
Hard limited to fifteen locations.
Couldn't figure out how to get the (undocumented?) browse button in the 'MFC VSListBox Control' working so I disabled it so user has to enter paths manually.
Was thrown together in a couple of hoursand intended for personal use only so it's not thoroughly tested. It has proven very stable though and uses < 1mb of RAM.
Not tested on anything other than Win 11.
