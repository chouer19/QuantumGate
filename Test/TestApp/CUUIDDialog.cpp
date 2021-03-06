// This file is part of the QuantumGate project. For copyright and
// licensing information refer to the license file(s) in the project root.

#include "pch.h"
#include "TestApp.h"
#include "CUUIDDialog.h"
#include "Common\UUID.h"

IMPLEMENT_DYNAMIC(CUUIDDialog, CDialogBase)

CUUIDDialog::CUUIDDialog(CWnd* pParent /*=nullptr*/)
	: CDialogBase(IDD_UUID_DIALOG, pParent)
{}

CUUIDDialog::~CUUIDDialog()
{}

void CUUIDDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUUIDDialog, CDialogBase)
	ON_BN_CLICKED(IDC_GENERATE_PEER_UUID, &CUUIDDialog::OnBnClickedGeneratePeerUUID)
	ON_BN_CLICKED(IDC_GENERATE_EXTENDER_UUID, &CUUIDDialog::OnBnClickedGenerateExtenderUUID)
	ON_BN_CLICKED(IDC_VALIDATE_UUID, &CUUIDDialog::OnBnClickedValidateUUID)
	ON_EN_CHANGE(IDC_UUID, &CUUIDDialog::OnEnChangeUuid)
	ON_BN_CLICKED(IDC_SAVE_PRIV_KEY, &CUUIDDialog::OnBnClickedSavePrivKey)
	ON_BN_CLICKED(IDC_SAVE_PUB_KEY, &CUUIDDialog::OnBnClickedSavePubKey)
END_MESSAGE_MAP()

BOOL CUUIDDialog::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// Init filter type combo
	const auto tcombo = (CComboBox*)GetDlgItem(IDC_ALG_COMBO);
	auto pos = tcombo->AddString(L"EDDSA_ED25519");
	tcombo->SetItemData(pos, static_cast<DWORD_PTR>(QuantumGate::UUID::SignAlgorithm::EDDSA_ED25519));
	pos = tcombo->AddString(L"EDDSA_ED448");
	tcombo->SetItemData(pos, static_cast<DWORD_PTR>(QuantumGate::UUID::SignAlgorithm::EDDSA_ED448));
	tcombo->SelectString(0, L"EDDSA_ED25519");

	return TRUE;
}

void CUUIDDialog::OnBnClickedGeneratePeerUUID()
{
	const auto sel = ((CComboBox*)GetDlgItem(IDC_ALG_COMBO))->GetCurSel();
	if (sel == CB_ERR)
	{
		AfxMessageBox(L"Please select a signing algorithm first.", MB_ICONINFORMATION);
		return;
	}

	const auto salg = static_cast<QuantumGate::UUID::SignAlgorithm>(((CComboBox*)GetDlgItem(IDC_ALG_COMBO))->GetItemData(sel));

	const auto[success, uuid, keys] = QuantumGate::UUID::Create(QuantumGate::UUID::Type::Peer, salg);
	if (success)
	{
		m_PeerKeys = std::move(keys);
		SetValue(IDC_PEER_UUID, uuid.GetString().c_str());

		GetDlgItem(IDC_SAVE_PRIV_KEY)->EnableWindow(true);
		GetDlgItem(IDC_SAVE_PUB_KEY)->EnableWindow(true);
	}
	else
	{
		m_PeerKeys.reset();
		SetValue(IDC_PEER_UUID, L"");

		GetDlgItem(IDC_SAVE_PRIV_KEY)->EnableWindow(false);
		GetDlgItem(IDC_SAVE_PUB_KEY)->EnableWindow(false);
	}
}

void CUUIDDialog::OnBnClickedGenerateExtenderUUID()
{
	[[maybe_unused]] const auto[success, uuid, keys] =
		QuantumGate::UUID::Create(QuantumGate::UUID::Type::Extender, QuantumGate::UUID::SignAlgorithm::None);

	if (success) SetValue(IDC_EXTENDER_UUID, uuid.GetString().c_str());
	else SetValue(IDC_EXTENDER_UUID, L"");
}

void CUUIDDialog::OnBnClickedValidateUUID()
{
	const auto uuidstr = GetTextValue(IDC_UUID);
	QuantumGate::UUID uuid;

	if (QuantumGate::UUID::TryParse(uuidstr.GetString(), uuid))
	{
		CString type = L"Unknown";

		switch (uuid.GetType())
		{
			case QuantumGate::UUID::Type::Peer:
				type = L"Peer";
				break;
			case QuantumGate::UUID::Type::Extender:
				type = L"Extender";
				break;
		}

		SetValue(IDC_VALIDATION_RESULT, L"UUID is valid; type is " + type);
	}
	else SetValue(IDC_VALIDATION_RESULT, L"UUID is invalid");
}

void CUUIDDialog::OnEnChangeUuid()
{
	SetValue(IDC_VALIDATION_RESULT, L"");
}

void CUUIDDialog::OnBnClickedSavePrivKey()
{
	const auto path = GetApp()->BrowseForFile(GetSafeHwnd(), true);
	if (path)
	{
		GetApp()->SaveKey(path->GetString(), m_PeerKeys->PrivateKey);
	}
}

void CUUIDDialog::OnBnClickedSavePubKey()
{
	const auto path = GetApp()->BrowseForFile(GetSafeHwnd(), true);
	if (path)
	{
		GetApp()->SaveKey(path->GetString(), m_PeerKeys->PublicKey);
	}
}
