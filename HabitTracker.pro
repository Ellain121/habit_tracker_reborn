TEMPLATE = subdirs

SUBDIRS += \
	app_core \
	app_desktop

app_desktop.depends = app_core
