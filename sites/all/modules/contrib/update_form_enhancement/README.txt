;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Update form enhancement module for Drupal
;; $Id: README.txt,v 1.1 2008/09/20 19:57:36 markuspetrux Exp $
;;
;; Original author: markus_petrux at drupal.org (September 2008)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

The update_form_enhancement module enhances update.php user interface by
separating modules that are up to date from those that are not.

It uses standard form_alter hook to alter the form that is displayed on second
step of update.php script to select modules that need to be updated.

When all modules are up to date, a message is displayed for the site admin and
the update form remains unchanged. The site admin can still reapply any update
task should (s)he needs to.

When all modules have pending update tasks, a message is displayed for the site
admin and the update form remains unchanged. This particular scenario may never
happen, but anyway...

When some modules have pending updates and some are up to date, they are
grouped into separated collapsible fieldset. The fieldset used to group modules
with pending updates is displayed first and expanded, so site admins can
easilly check what's pending. The fieldset used to group up to date modules is
rendered last and collapsed.



INSTALLATION
============

- Copy the contents of this package to your modules directory.
- Goto admin/build/modules to install the module.
- That's all.


SUPPORT
=======

- Please use the issue tracker of the project at drupal.org:

  http://drupal.org/project/update_form_enhancement
