PROG=proj

SRCS = proj.c dispatcher.c draw.c kbc_handler.c graphics.c mouse.c keyboard.c timer.c projectile_handler.c player_handler.c rtc.c colisions.c main_pause_menu.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
