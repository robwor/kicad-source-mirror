/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2017 Seth Hillbrand <hillbrand@ucdavis.edu>
 * Copyright (C) 2014 KiCad Developers, see CHANGELOG.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef __dialog_edit_line_style__
#define __dialog_edit_line_style__


/**
 * @file dialog_edit_line_style.h
 * @brief Subclass of DIALOG_EDIT_LINE_STYLE_BASE, which is generated by wxFormBuilder.
 */

#include <dialog_edit_line_style_base.h>
#include <sch_line.h>

class WIDGET_EESCHEMA_COLOR_CONFIG;

class DIALOG_EDIT_LINE_STYLE : public DIALOG_EDIT_LINE_STYLE_BASE
{
public:
    DIALOG_EDIT_LINE_STYLE( wxWindow* parent );

    void SetWidth( const wxString& aWidth ) { m_lineWidth->SetValue( aWidth ); }
    void SetDefaultWidth( const wxString& aWidth ) { m_defaultWidth = aWidth; }
    wxString GetWidth() const { return m_lineWidth->GetValue(); }

    COLOR4D GetColor() const { return m_selectedColor; }
    void SetColor( const COLOR4D& aColor, bool aRefresh );
    void SetDefaultColor( const COLOR4D& aColor );

    void SetStyle( const int aStyle );
    void SetDefaultStyle( const int aStyle ) { m_defaultStyle = aStyle; }
    int GetStyle();

    void SetLineWidthUnits(const wxString& aUnits)
    {
        m_staticWidthUnits->SetLabel( aUnits );
    }

private:
    int m_defaultStyle;
    wxString m_defaultWidth;
    COLOR4D m_defaultColor;
    COLOR4D m_selectedColor;

    void resetDefaults( wxCommandEvent& event ) override;
    void onColorButtonClicked( wxCommandEvent& aEvent ) override;
    void updateColorButton( COLOR4D& aColor );
};

#endif // __dialog_edit_line_style__
