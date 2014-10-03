;;
;;  slovak.nsh
;;
;; Ivan Masar <helix84@users.sourceforge.net>
;;
;;  Default language strings for the Windows Dia NSIS installer.
;;  Windows Code page: 1250
;;
;;  Version 2
;;  Note: If translating this file, replace "!define DIA_MACRO_DEFAULT_STRING"
;;  with "!define".

; Make sure to update the DIA_MACRO_LANGUAGEFILE_END macro in
; langmacros.nsh when updating this file

; GTK+ was not found
!define DIA_NO_GTK			"Pros�m, nain�talujte si GTK+ verziu 2.6.0 alebo vy��iu. N�jdete ju na dia-installer.sourceforge.net"

; Don't install over pre 0.95 versions
!define DIA_NO_INSTALL_OVER		"Pros�m, �plne odstr�nte star� in�tal�cie Dia alebo nain�talujte Dia na in� miesto."

; License Page
!define DIA_LICENSE_BUTTON		"�alej >"
!define DIA_LICENSE_BOTTOM_TEXT		"$(^Name) je uvo�nen� za podmienok licencie GPL. Licencia je tu poskytnut� len pre informat�vne ��ely. $_CLICK"

; Components Page
!define DIA_SECTION_TITLE			"Dia - editor diagramov (povinn�)"
!define TRANSLATIONS_SECTION_TITLE	"Preklady"
!define TRANSLATIONS_SECTION_DESCRIPTION  "Voliteln� preklady pou��vate�sk�ho rozhrania Dia"
!define DIA_SECTION_DESCRIPTION		"Hlavn� s�bory a kni�nice Dia"
!define PYTHON_SECTION_TITLE              "Z�suvn� modul Python"
!define PYTHON_SECTION_DESCRIPTION              "Podpora skriptovacieho jazyka Python 2.3. Nevyberajte t�to vo�bu, ak nie je Python nain�talovan�."

; Installer Finish Page
!define DIA_FINISH_VISIT_WEB_SITE		"Nav�t�vte webstr�nku Dia pre Windows"

; DIA Section Prompts and Texts
!define DIA_UNINSTALL_DESC		"Dia (iba odstr�ni�)"
!define DIA_PROMPT_WIPEOUT		"V� star� adres�r Dia bude zmazan�. Chcete pokra�ova�?$\r$\rPozn.: V�etky ne�tandardn� z�suvn� moduly, ktor� ste mo�no nain�talovali, bud� zmazan�.$\rPou��vate�sk� nastavenia Dia nebud� ovplyvnen�."
!define DIA_PROMPT_DIR_EXISTS		"In�tala�n� adres�r, ktor� ste uviedli, u� existuje. V�etok obsah$\rbude zmazan�. Chcete pokra�ova�?"

; Uninstall Section Prompts
!define un.DIA_UNINSTALL_ERROR_1		"Program na dein�tal�ciu nena�iel z�znamy Dia v registri.$\rJe pravdepodobn�, �e t�to aplik�ciu nain�taloval in� pou��vate�."
!define un.DIA_UNINSTALL_ERROR_2		"Nem�te povolenie odin�talova� t�to aplik�ciu."
!define un.DIA_UNINSTALLATION_WARNING	"T�mto sa �plne zma�e $INSTDIR a v�etky jeho podadres�re. Pokra�ova�?"
!define un.DIA_DOTDIA_WARNING		"T�mto sa �plne zma�e $PROFILE\.dia a v�etky jeho podadres�re. Pokra�ova�?"
