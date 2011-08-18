$Id: README.txt,v 1.2.2.2 2009/12/30 21:03:28 markdrupal Exp $
################################################################################
HISTORY
################################################################################
The Upload Replace grew out of a need of mine to have bookmarkable files on a
website I was running.  The default behavior of filefield CCK is to rename new
files to <filename>_0.<ext> if that file name is in use.

In my case I needed the newest version of the file to be called <filename>.<ext>
so that my users can bookmark the file and always get the most recent version
when they use their bookmarks.

As well, this needed to work with node revisions so if a incorrect file was
uploaded an administrator could revert to the correct version and the filename
would be updated.

################################################################################
USAGE
################################################################################
filefield CCK is required for this module( http://drupal.org/project/filefield )
Drupal's upload module is NOT compatible with upload_replace.

1.Enable the upload_replace module on the modules page.
2.Your done

There currently are not any options for this module, just turn it on and it
should work.  It uses the HOOK_file_update to do it's magic.

################################################################################
ISSUES
################################################################################
There may be issues with some modules that cache files or thumbnails. If you
find a module that has cache problems after uploading a newer version of a file
please post an issue.

Please use the project issue tracker to report problems
http://drupal.org/project/issues/upload_replace

Project page is here:
http://drupal.org/project/upload_replace

################################################################################
AUTHOR
################################################################################
Original author is
USERNAME:markDrupal
FULLNAME:Mark Crandell
EMAIL:markc09@gmail.com
