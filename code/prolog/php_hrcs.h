#ifndef PHP_HRCS_H
#define PHP_HRCS_H 1

#define PHP_HRCS_VERSION "1.0"
#define PHP_HRCS_EXTNAME "hrcs"

PHP_FUNCTION(hrcs);

extern zend_module_entry hrcs_module_entry;
#define phpext_hrcs_ptr &hrcs_module_entry

#endif
