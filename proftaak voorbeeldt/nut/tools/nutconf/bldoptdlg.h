#ifndef BLDOPTDLG_H_
#define BLDOPTDLG_H_

/* ----------------------------------------------------------------------------
 * Copyright (C) 2004 by egnite Software GmbH
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 * Parts are
 *
 * Copyright (C) 1998, 1999, 2000 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 */

/*
 * $Log: bldoptdlg.h,v $
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/wx.h>
#include <wx/config.h>


class CBuildOptionsDialog: public wxPanel
{
DECLARE_CLASS(CBuildOptionsDialog)
public:
    CBuildOptionsDialog(wxWindow* parent);
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    wxComboBox *m_cbxPlatform;
    wxTextCtrl *m_entSourceDir;
    wxTextCtrl *m_entInclFirstDir;
    wxTextCtrl *m_entInclLastDir;
    wxTextCtrl *m_entBuildDir;
    wxTextCtrl *m_entInstallDir;
    wxString m_lastSourceDir;
    void OnBrowseBuildPath(wxCommandEvent& event);
    void OnBrowseInstallPath(wxCommandEvent& event);
    void OnBrowseSourceDir(wxCommandEvent& event);
    void OnPlatformEnter(wxCommandEvent& event);
    void OnSourceDirChange(wxCommandEvent& event);
    void PopulatePlatform();

    DECLARE_EVENT_TABLE()

};

#endif
