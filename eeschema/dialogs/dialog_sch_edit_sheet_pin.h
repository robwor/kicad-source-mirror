/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2010 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2018 KiCad Developers, see AUTHORS.txt for contributors.
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

#ifndef __dialog_sch_edit_sheet_pin__
#define __dialog_sch_edit_sheet_pin__


/**
 * @file
 * Subclass of DIALOG_SCH_EDIT_SHEET_PIN_BASE, which is generated by wxFormBuilder.
 */


#include <dialog_sch_edit_sheet_pin_base.h>
#include <sch_text.h>   // enum PINSHEETLABEL_SHAPE definition


class DIALOG_SCH_EDIT_SHEET_PIN : public DIALOG_SCH_EDIT_SHEET_PIN_BASE
{
public:
    DIALOG_SCH_EDIT_SHEET_PIN( wxWindow* parent );

    // Set the label text in m_textName.
    // Because m_textName uses a wxTextValidator, the label text must be stored in the
    // validator string
    void SetLabelName( const wxString& aName ) { m_labelString = aName; }
    wxString GetLabelName() const { return m_textName->GetValue(); }

    void SetTextHeight( const wxString& aHeight ) { m_textHeight->SetValue( aHeight ); }
    wxString GetTextHeight() const { return m_textHeight->GetValue(); }

    void SetTextWidth( const wxString& aWidth ) { m_textWidth->SetValue( aWidth ); }
    wxString GetTextWidth() const { return m_textWidth->GetValue(); }

    void SetConnectionType( PINSHEETLABEL_SHAPE aType ) { m_choiceConnectionType->SetSelection( aType ); }
    /// @todo move cast to widget
    PINSHEETLABEL_SHAPE GetConnectionType() const { return static_cast<PINSHEETLABEL_SHAPE>( m_choiceConnectionType->GetCurrentSelection() ); }

    void SetTextHeightUnits( const wxString& aUnit ) { m_staticHeightUnits->SetLabel( aUnit ); }
    void SetTextWidthUnits( const wxString& aUnit ) { m_staticWidthUnits->SetLabel( aUnit ); }

private:
	void onOKButton( wxCommandEvent& event ) override;
};

#endif // __dialog_sch_edit_sheet_pin__
