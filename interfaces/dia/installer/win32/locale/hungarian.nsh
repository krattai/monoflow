;;
;;  hungarian.nsh
;;
;;  Gabor Kelemen  <kelemeng@gnome.hu>
;;
;;  Hungarian strings for the Windows Dia NSIS installer.
;;  Windows Code page: 1250
;;
;;  Version 2
;;  Note: If translating this file, replace "!insertmacro DIA_MACRO_DEFAULT_STRING"
;;  with "!define".

; Make sure to update the DIA_MACRO_LANGUAGEFILE_END macro in
; langmacros.nsh when updating this file

; GTK+ was not found
!define DIA_NO_GTK			"Telep�tse a GTK+ 2.6.0 vagy �jabb verzi�j�t. Ez el�rhet� a dia-installer.sourceforge.net oldalr�l."

; Don't install over pre 0.95 versions
!define DIA_NO_INSTALL_OVER		"T�vol�tsa el a 0.95 alatti verzi�sz�m� Dia telep�t�seket, vagy v�lasszon m�sik helyet."

; License Page
!define DIA_LICENSE_BUTTON		"Tov�bb >"
!define DIA_LICENSE_BOTTOM_TEXT		"A $(^Name) a GPL licenc alatt ker�l kiad�sra. Az itt l�that� licenc csak t�j�koztat�si c�lt szolg�l. $_CLICK"

; Components Page
!define DIA_SECTION_TITLE			"Dia diagramszerkeszt� (k�telez�)"
!define TRANSLATIONS_SECTION_TITLE	"Ford�t�sok"
!define TRANSLATIONS_SECTION_DESCRIPTION  "A Dia felhaszn�l�i fel�let�nek ford�t�sai"
!define DIA_SECTION_DESCRIPTION		"Alapvet� Dia f�jlok �s programk�nyvt�rak"
!define PYTHON_SECTION_TITLE        "Python b�v�tm�ny"
!define PYTHON_SECTION_DESCRIPTION  "A Python parancsnyelv 2.3 v�ltozat�nak t�mogat�sa. Ne v�lassza ki, ha a Python nincs telep�tve."
 

; Installer Finish Page
!define DIA_FINISH_VISIT_WEB_SITE		"Keresse fel a Dia for Windows weboldal�t"

; DIA Section Prompts and Texts
!define DIA_UNINSTALL_DESC		"Dia (csak elt�vol�t�s)"
!define DIA_PROMPT_WIPEOUT		"A r�gi Dia k�nyvt�r�nak elt�vol�t�s�ra k�sz�l. Folytatni akarja?$\r$\rMegjegyz�s: Az esetlegesen telep�tett nem szabv�nyos b�v�tm�nyek t�rl�sre ker�lnek.$\rA Dia felhaszn�l�i be�ll�t�sait ez nem �rinti."
!define DIA_PROMPT_DIR_EXISTS		"A megadott telep�t�si k�nyvt�r m�r l�tezik. A tartalma$\rt�rl�sre ker�l. Folytatni akarja?"

; Uninstall Section Prompts
!define un.DIA_UNINSTALL_ERROR_1		"Az elt�vol�t� program nem tal�lja a Dia regisztr�ci�s adatb�zis-bejegyz�seit. $\rVal�sz�n�leg m�sik felhaszn�l� telep�tette ezt az alkalmaz�st."
!define un.DIA_UNINSTALL_ERROR_2		"Nincs jogosults�ga elt�vol�tani ezt az alkalmaz�st."
!define un.DIA_UNINSTALLATION_WARNING	"Ez teljesen t�rli a(z) $INSTDIR k�nyvt�rat �s minden alk�nyvt�r�t. Folytatja?"
!define un.DIA_DOTDIA_WARNING		"Ez teljesen t�rli a(z) $PROFILE\.dia k�nyvt�rat �s minden alk�nyvt�r�t. Folytatja?"
