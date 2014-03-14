downloadCM
==========
Application to download CM updates for a specified model (or series of models) to the local system.

Build Instructions
==================
I've included the project files for both XCode and Visual Studio 2013 here, but if you want to go
completely home-grown and not use those files, compile the .cpp files into objects, make sure that
you link libcurl for the MacOS version and the proper wininet library for the WIN32 version and you
should get executable code.

I'm running Win32 natively on a little netbook for that environment, so I haven't tested anything
in 64 bit yet, but the code works fine on the Mac for 64 bit so I think it is just a matter of
linking the correct library and the compiler building the files for 64 vs 32bit.

Version History
===============
v0.1 Initial Release
 * Basic funcinality is working.
   * Gets the HTML from CyanogenMod site, parses for updates.
   * If it finds one, downlaods it.
 * KNOWN ISSUES
   * Subfolder support (-s option) does not yet work.
   * No progress bar for -v option when downloading.
   * Does not pass error codes to console especially in WIN32 (even for -v).

To Do List
==========
 * Add support for DropBox Uploading
   * Will have to deal with obtaining authentication information
   * Don't really want to do it the same way I did it for the .py script, or call that script
     but I may have to, I'd also rather not have it call out to the web, besides who getting this
     program wouldn't be geek enough?
 * Finish sub-path support
 * Add progress bars for running in verbose mode
 * Add support for config files
   * Will need a dropbox.conf to store key information.
   * Will need a pushover.conf to store key information.
   * Could use a devices.list (supported by -d @filename.list argument) for people who
     have a lot of devices to download for.
   * Also going to look into a download.conf file to store default options, perhaps
     put everything into that one file with directives in it
     * Probably just make them use the same identifiers we'd pass to the command as arguments
       that would be most consistant
