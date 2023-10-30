= Lobbyについて
この章ではLobby機能について話していきます。

== 概要 
オンラインゲームをやったことある方ならイメージしやすいと思われますが、UGSのLobbyでも似たようなことができます。
PlayersLinkでは、下図のようなスタイルのロビー画面を採用しています。

#@#TODO:一般的なゲームのロビー画面
//image[2_1_1_lobby][図の表示]{
  2023年時点で一般的なゲームのロビー画面
//}


== 困ったこと
UGSのLobby機能は非常に優れていますが、私は開発していくなかで痒いところに手が届かないなといった印象を受けました。
というのも、Lobby機能ではロビー内で起こりうるイベントを監視できないからです。

例えば、LobbyクラスにはOnJoinedやOnDiscconctedなどのイベント関数が公開されていません。
そのため、プレイヤーがロビーに参加したかどうか切断したかどうかを検知する様な実装を各々で施す必要があります。
ここはけっこう苦悩しましたが、一応Lobbyシステムで解決できる方法を見つけました。

私はこの問題を解決するためにLobbyシステムのUpdateLobbyDataとUpdatePlayerDataを使いました。
この二つのメソッドは、いずれもロビーにデータを書き込む処理です。
まず、プレイヤーは一定時間毎にタイムスタンプをロビーのプレイヤーデータとして書き込みます。

#@#TODO:一般的なゲームのロビー画面
//image[20220315105706][図の表示]{
  a2023年時点で一般的なゲームのロビー画面1
//}

一方のプレイヤーは、最新のロビーデータを取得して、他のプレイヤーが書き込んだタイムスタンプを確認します。この時、タイムスタンプがしよ
Unity側で想定しているやり方なのかは分かりませんが