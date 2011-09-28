/* ****************************************************************************
 * eID Middleware Project.
 * Copyright (C) 2008-2011 FedICT.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version
 * 3.0 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, see
 * http://www.gnu.org/licenses/.
**************************************************************************** */

#include <gtk/gtk.h>
#include <gtk/gtkenums.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gi18n.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"
#include <locale.h>
#include "single_dialog.h"

#define EXIT_OK		0
#define EXIT_CANCEL 1
#define EXIT_ERROR	2

enum { MSG_CHANGE_PIN_CODE=1, MSG_PLEASE_CHANGE_PIN };
char* beid_messages[4][3]={
                                    "en",   "beID: Change PIN Code",      		"Please change your PIN code on the secure pinpad reader:\n[%s]..",
                                    "nl",   "beID: Wijziging PINcode",        	"Gelieve uw PIN code op de beveiligde kaartlezer:\n[%s]\nte willen wijzigen.",
                                    "fr",   "beID: Changement de code PIN",    	"Veuillez changer votre code PIN sur le lecteur securise\n[%s]..",
                                    "de",   "beID: Change PIN Code",      		"Please change your PIN code on the secure pinpad reader:\n[%s].."
                              };

#include "beid-i18n.h"


// struct holding all the runtime data, so we can use callbacks without global variables
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	GtkWidget  *dialog;
} PinDialogInfo;

// event handler for delete-event. always approves the deletion
///////////////////////////////////////////////////////////////
static gboolean on_delete_event( GtkWidget *widget, GdkEvent* event, gpointer pindialog)
{
    return TRUE;
}

int main(int argc, char* argv[])
{
	abort_if_parent_not_considered_secure();

	char			pid_path[PATH_MAX];
	int 			return_value=EXIT_ERROR;
	PinDialogInfo 	pindialog;									// this struct contains all dialog objects

    gtk_init(&argc,&argv);										// initialize gtk+
	
	// create new message dialog with CANCEL button in standard places, in center of user's screen
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char message[1024];
	if((argc==2) && (argv[1]!=NULL) && (strlen(argv[1])>0))
	{
		snprintf(message,sizeof(message)-2,_MSG_(MSG_PLEASE_CHANGE_PIN),argv[1]);
	}
	else
	{
		fprintf(stderr,"Incorrect Parameter for <description of SPR>\n");
		exit(1);
	}

    pindialog.dialog=gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_NONE,message);
	gtk_dialog_set_default_response(GTK_DIALOG(pindialog.dialog),GTK_RESPONSE_OK);
    gtk_window_set_title(GTK_WINDOW(pindialog.dialog),_MSG_(MSG_CHANGE_PIN_CODE));
    gtk_window_set_position(GTK_WINDOW(pindialog.dialog), GTK_WIN_POS_CENTER);
    g_signal_connect (pindialog.dialog, "delete-event", G_CALLBACK (on_delete_event),&pindialog);

	// show all these widgets, and run the dialog as a modal dialog until it is closed by the user
	//////////////////////////////////////////////////////////////////////////////////////////////    

    gtk_widget_show_all(GTK_WIDGET(pindialog.dialog));
	
    switch(gtk_dialog_run(GTK_DIALOG(pindialog.dialog)))
	{
		case GTK_RESPONSE_CANCEL:				// if the use chose CANCEL
			printf("CANCEL\n");
			return_value=EXIT_OK;				// output CANCEL and return OK (cancel is not an error)
		break;

		default:								// otherwise
			printf("ERROR\n");
			return_value=EXIT_ERROR;			// output and return ERROR
		break;
	}

	// properly dispose of the dialog (which disposes of all it's children), and exit with specific return value
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	gtk_widget_destroy(pindialog.dialog);
	exit(return_value);
}



