
= はじめに

画面の向こう側の誰かを打ち負かしてニヤリとしたり、
仲間と協力して強力なモンスターに立ち向かうのって楽しいですよね。
オンラインゲームはそんな体験を私たちに与えてくれます。
近年はUnityやUnrial Engineの登場によって、ゲーム会社ではなくインディーゲーム開発者の方々が増えてきました。
私もそんな一人で新卒入社した会社に入ってからたまたまUnityを知る機会があり、
ゲーム開発そのものにどんどんのめり込んでいきました。
そんな中で開発したPieceWarsというゲームをオンライン化したいという思いから、今回UGSの勉強とPlayersLinkの開発が始まりました。

== 概要
UGSを使ったオンラインゲームの開発自体はネット上にけっこうな情報が存在しており、
正直この本を読まなくても基本的な実装は中級レベルの開発者であれば可能だと思います。
極論を言えば、エラーが発生しないことを前提とした"繋げるだけ"の処理を実装であれば誰でも出来てしまいます。
しかし、オンラインゲームの開発で意識すべき重要なことは適切なエラーハンドリングだと私は考えます。
家庭における有線環境であれば基本的に問題はないのですが、街中や電車の中で通信を行う場合は通信が不安定なため
すぐにエラーとなってしまいます。
そのため、必要に応じてリトライ処理をしてあげたり、許容範囲を超える通信状況の悪さによってはプレイヤーにエラーを通知してあげなければなりません。
PlayersLinkでは、その辺りを吸収できるように適宜リトライとエラーハンドリングを埋め込んで開発者がそれほど多くのことを考えなくてもいいような設計にしています。
#@#ある程度プログラムを理解しているひとであれば、「このあたりは失敗しやすいだろうからリトライ処理を入れよう」と勘が働くと思いますが、
#@##あまり慣れていない人にとってはあちこちに
本書を読んでいく中で「ああ、この辺にリトライ処理を入れて、このあたりはエラーハンドリングした方がいいんだろうな」という勘どころが身につけば幸いです。

== 基本パターン
UGSでマッチメイキングで行うべきことは主に以下の3つです。
*認証
*ロビーの作成/参加
*Relayサーバーの作成/参加

//image[1_1_1_基本的にやるべきこと][マッチメイキングで基本的にやるべきこと]{
//}

「UGSの概要としては以上です！」と言いたいくらいこれが全てなのですが、せっかく本書を読んで下さっている皆様のためにももう少しご説明したいと思います。

==認証
UGSの機能を呼び出す際は必ず認証を行う必要があります。
認証には匿名認証、Google認証、Apple認証があります。
実のところ、匿名認証しか試したことないのですが、匿名認証自体は簡単だったので他の認証もそんなに難しくないと思われます。

今後、触れることがあれば情報を提供していきますが本書では割愛いたします。
Lobby、Relayを単体で使用する場合もこれを行います。
トークンなどを特別に管理する必要はなく認証クラスの匿名認証メソッドを呼び出しさえすれば、Lobby/Relay共に使える様になります。
//image[1_3_1_認証について][認証]{
//}

==Lobby
Lobby機能を使うと最近のゲームで一般的なロビーが作れるようになります。

//image[1_1_2_一般的なロビー画面][一般的なロビー画面]{
  2023年11月時点で一般的なゲームのロビー画面
//}

しかし、画面等が用意されているわけではなく、あくまで「サーバー上でプレイヤー同士の待ち合わせができる」機能が提供されているだけです。
そのぐらい最初から作っておいてよという感じですが、このあたりは自分で実装する必要がありました。

PlayersLinkでは、このあたりを開発者があまりやらなくていい様に画面を用意したり、待ち合わせ機能をワンボタンで実行できるようになってます。
具体的には、ロビーの入室/退室、準備状況をプレイヤー間で共有ができます。

//image[1_4_1_ロビーでやること][ロビーでやること]{
//}
//image[1_4_2_ロビーにおける処理の流れ_点呼][ロビーにおける処理の流れ 準備状況の確認]{
//}

全員の準備が整ったらいよいよ対戦開始ということで次はRelayの出番です。

==Relay
Relayは日本語で”中継”を意味します。
後述のNetcode for Objectsではホストとなるプレイヤーがサーバーの役割を担います。
しかし、そのままでは他のクライアントとなるプレイヤーが接続できません。
IPアドレスが分からないからです。それを解決するのがRelayサーバーの役割です。

流れとしては、
最初にホストとなるプレイヤーがRlayサーバーを立ち上げます。
この時、RelayサーバーからJoinコードが発行されます。
この発行されたJoinコードをどうにかしてクライアント側に渡します。
クライアントはRelayサーバーにこのJoinコードを渡すことでホストのIPアドレス(それとポート番号)を知ることができます。
これにより、無事にホストとクライアントが同期できる様になりました。

途中、Joinコードの受け渡しの説明についてあえて説明を省いたため、もしかしたら「ん？」と思われたか方もいらっしゃるかもしれませんがこれからご説明いたしますので安心してください。
Joinコードをクライアント側のプレイヤーに渡す方法は二つあります。
一つは、ホストがLobbyデータに書き込みを行い、クライアントがLobbyデータを読み込んで取得する方法です。
//image[1_5_1_Relay概要_ホストの動き][ホストの動き]{
  2023年時点で一般的なゲームのロビー画面
//}

//image[1_5_2_Relay概要_クライアントの動き][クライアントの動き]{
  2023年時点で一般的なゲームのロビー画面
//}


二つ目は、発行されたJoinコードをゲーム画面上からクリップボードにコピーできるようにしておき、DiscordやLineなどで直接クライアントに通知する方法があります。
この方法だともはやLobbyは必要ありません。これはプライベートマッチを実装する際に有効な手段です。

//image[1_5_3_よくあるJoinコードのクリップボード][よくあるJoinコードのクリップボード]{
  2023年時点で一般的なゲームのロビー画面
//}

AMONG USやStumble Guysで見られるようなあれです。
おそらく、これらのゲームもUGSのLobby/Relay機能を使っていると思います。

ここまでざっくりとUGSの提供する機能をざっと見てきました。次の章からLobbyとRelayの実装方法についてPlayersLinkを例に見ていきます。

