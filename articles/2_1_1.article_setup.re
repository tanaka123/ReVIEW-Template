= 事前準備
この章ではUGSを使用する上で必要な準備について書いていきます。
このあたりはネット上に情報が山ほどあるので簡単な説明とさせていただきます。

== 手順
=== プロジェクトの作成
これは言わずもがな。今回はUnity2022.3.10f1を使用します。

=== UGSの登録
UGSの登録が必要ですので下記URLより、UGSへの登録を行う。
@<href>{https://dashboard.unity3d.com/gaming} 

=== UGSとプロジェクトをリンク
プロジェクト作成後、Services>General Settingsから下図ウィンドウを開きます。
//indepimage[1_2_1_サービスリンク1][]{
//}

次にOrganizationsを設定します。
//indepimage[1_2_1_サービスリンク2][]{
//}

Create Project IDを押せば、自動的にUGS上にプロジェクト情報が登録され、UGSとの連携が可能になります。
//indepimage[1_2_1_サービスリンク3][]{
//}

=== UGSでLobbyとRelayの有効化
UGSの登録時に使用したURLからログインして、LobbyとRelayを有効化してください。


=== パッケージのインポート
下記のパッケージをそれぞれインストールされていることを確認してください。
インストールされていない場合はインストールしてください。
 * Authentication 2.7.2
 * Lobby 1.1.0
 * Relay 1.0.5
 * UTP 2.1.0
 * Netcode for GameObjects 1.6.0
 * Multiplayer Samples Utilities 1.6.1
 * ParrelSync 1.5.2
 * UniTask 2.4.1

=== ParrelSyncについて
UnityEditorを複製してマルチプレイのテストを可能にするライブラリツールです。
いちいちビルドしなくてもこのツールを使えば、簡単に複数人でアクセスを想定したテストができます。

=== WebGLでのビルドについて
RelayでWebScketを使用します。そのため、Unityのシーン上でNetworkMangerのUse Web Scketを有効化する必要があります。
PlayersLinkではこの辺りの処理を自動化しています。
また、WebGLでは非同期処理ができません。サンプルではTaskを使用していますが、PlayersLinkではUniTaskに切り替えています。

=== Netcode for GameObjectsとは
Lobby,Relayを使用してプレイヤー同士を同期した後、
シーン上のプレイヤーの位置情報(Transform)を同期したり、RPCを使用するためのライブラリです。
このあたりは通常のUnity開発をやったことがある方ならある程度とっつきやすいのではないかと思います。

