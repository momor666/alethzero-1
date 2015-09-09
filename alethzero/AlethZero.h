/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file AlethZero.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#pragma once

#ifdef Q_MOC_RUN
#define BOOST_MPL_IF_HPP_INCLUDED
#endif

#include <map>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QAbstractListModel>
#include <QtCore/QMutex>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <libdevcore/RLP.h>
#include <libethcore/Common.h>
#include <libethcore/KeyManager.h>
#include <libethereum/State.h>
#include <libethereum/Executive.h>
#include <libwebthree/WebThree.h>
#include <libsolidity/CompilerStack.h>
#include <libaleth/NatspecHandler.h>
#include <libaleth/Common.h>
#include <libaleth/Aleth.h>
#include "ZeroFace.h"
#include "Connect.h"
#include "Plugin.h"

class QListWidgetItem;
class QActionGroup;

namespace Ui { class Main; }

namespace dev
{

class SafeHttpServer;

namespace eth
{
class Client;
class State;
}

namespace aleth
{
namespace zero
{

class WebThreeServer;
class SettingsDialog;
class NetworkSettings;

class AlethZero: public ZeroFace
{
	Q_OBJECT

public:
	AlethZero();
	~AlethZero();

	WebThreeServer* web3Server() const override { return m_server.get(); }
	dev::SafeHttpServer* web3ServerConnector() const override { return m_httpConnector.get(); }

	AlethFace const* aleth() const { return &m_aleth; }
	AlethFace* aleth() { return &m_aleth; }

public slots:
	void note(QString _entry);
	void debug(QString _entry);
	void warn(QString _entry);
	QString contents(QString _file);

	void onKeysChanged();

private slots:
	// Application
	void on_about_triggered();
	void on_quit_triggered() { close(); }

	// Network
	void on_go_triggered();
	void on_net_triggered();
	void on_connect_triggered();

	// View
	void on_refresh_triggered();
	void on_preview_triggered();

	// Account management
	void on_killAccount_triggered();
	void on_reencryptKey_triggered();
	void on_reencryptAll_triggered();
	void on_exportKey_triggered();
	void on_ourAccounts_itemClicked(QListWidgetItem* _i);
	void on_ourAccounts_doubleClicked();

	// Special (debug) stuff
	void on_confirm_triggered();
	void on_settings_triggered();
	void refreshAll();

	void onBeneficiaryChanged();

private:
	void allStop() override;
	void carryOn() override;

	template <class P> void loadPlugin() { Plugin* p = new P(this); initPlugin(p); }
	void initPlugin(Plugin* _p);
	void finalisePlugin(Plugin* _p);
	void unloadPlugin(std::string const& _name);
	void addSettingsPage(int _index, QString const& _categoryName, std::function<SettingsPage*()> const& _pageFactory) override;

	void createSettingsPages();

	void setNetPrefs(NetworkSettings const& _settings);
	NetworkSettings netPrefs() const;

	void readSettings(bool _skipGeometry = false, bool _onlyGeometry = false);
	void writeSettings();

	void installWatches();

	void refreshNetwork();
	void refreshMining();
	void refreshBlockCount();
	void refreshBalances();

	std::string getPassword(std::string const& _title, std::string const& _for, std::string* _hint = nullptr, bool* _ok = nullptr);

	std::unique_ptr<Ui::Main> m_ui;

	std::unique_ptr<dev::SafeHttpServer> m_httpConnector;	// TODO: move into Aleth, eventually.
	std::unique_ptr<WebThreeServer> m_server;	// TODO: move into Aleth, eventually.

	Aleth m_aleth;

	SettingsDialog* m_settingsDialog = nullptr;
};

}
}
}
