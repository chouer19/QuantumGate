// This file is part of the QuantumGate project. For copyright and
// licensing information refer to the license file(s) in the project root.

#pragma once

#include "CDialogBase.h"

class CUUIDDialog final : public CDialogBase
{
	DECLARE_DYNAMIC(CUUIDDialog)

public:
	CUUIDDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUUIDDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UUID_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedGeneratePeerUUID();
	afx_msg void OnBnClickedGenerateExtenderUUID();
	afx_msg void OnBnClickedValidateUUID();
	afx_msg void OnEnChangeUuid();
	afx_msg void OnBnClickedSavePrivKey();
	afx_msg void OnBnClickedSavePubKey();

private:
	std::optional<QuantumGate::UUID::PeerKeys> m_PeerKeys;
};
