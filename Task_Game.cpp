//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"

#include  "randomLib.h"
#include  <string>
#include  "sound.h"

#include  "Task_Background.h"
#include  "Task_StartButton.h"
#include  "Task_Player.h"
#include  "Task_Apple.h"
#include  "Task_UI.h"
#include  "Task_Apple_Generator.h"
#include  "debug.h"


namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->scoreFont = DG::Image::Create("./data/image/font_text.png");
		this->imgGameOver = DG::Image::Create("./data/image/gameOver.png");

	//	bgm::LoadFile("MainBGM", "./data/sound/bgm/弾むキモチ_2.wav");
	//	bgm::Play("MainBGM");


		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->scoreFont.reset();
		this->imgGameOver.reset();

	//	bgm::Stop("MainBGM");

		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();
		this->render2D_Priority[1] = -1.f;

		//★データ初期化
		
		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//★タスクの生成
		auto debug = debug::Object::Create(true);
		auto bg = BG::Object::Create(true);
	//	auto player = Player::Object::Create(true);
		auto ui = UI::Object::Create(true);
	//	auto ag = AppleGenerator::Object::Create(true);

		

		this->appleCnt = 0;
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->TestFont.reset();
		ge->mouse.reset();

		ge->KillAll_G("MainGame");
		ge->KillAll_G("BG");
		ge->KillAll_G("Player");
		ge->KillAll_G("UI");
		ge->KillAll_G("Apple Generator");
		ge->KillAll_G("Apple");
		ge->KillAll_G("debug");
		ge->KillAll_G("Start");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		/*	auto player = ge->GetTask<Player::Object>("Player", "NoName");
			if (player != nullptr) {
				next->points = player->points;
			}*/
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();

		auto inp = ge->in1->GetState( );
	//	auto player = ge->GetTask<Player::Object>("Player", "NoName");
	//	auto apple = ge->GetTask<Apple::Object>("Apple", "NormalApple");
		auto debug = ge->GetTask<debug::Object>("debug", "NoName");


		if (ms.LB.down) {
			auto spawnapple = Apple::Object::Create(true);
			spawnapple->pos = ML::Vec2(ms.pos.x - spawnapple->hitBase.w / 2, ms.pos.y - spawnapple->hitBase.h / 2);
			this->appleCnt++;
		}




		//-------------------デバッグ機能対応------------------
		if (inp.SE.down) {
			this->IsDebug = !this->IsDebug; //maybe useless
			if (this->IsDebug) {
				debug->isOn = true;
			}
			else {
				debug->isOn = false;
			}
		}

		//---------------------------------------------------


		//if (inp.ST.down) {
		//	ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
		//	ge->CreateEffect(99, ML::Vec2(0, 0));
		//}
		//if (player->isGameOver) {
		//	debug->hitFlag = true; //debug

		//	if (true == inp.ST.down) {
		//		player->isGameOver = false;
		//		ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
		//		ge->CreateEffect(98, ML::Vec2(0, 0));
		//	}
		//	if (ge->getCounterFlag("test") == ge->LIMIT) {
		//		this->Kill();
		//	}
		//}
		//else { debug->hitFlag = false; }
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	//	auto player = ge->GetTask<Player::Object>("Player", "NoName");

		std::string score = "Score:" + to_string(this->appleCnt);
		int x = 0;
		int charWidth = 32;
		int charHeight = 32;

		int screenWidth = ge->screenWidth;
		int totalTextWidth = static_cast<int>(score.size()) * charWidth;
		int startX = screenWidth - totalTextWidth - 20; // 20px margin from the right

		for (char c : score) {
			int charIndex = static_cast<int>(c); // No offset

			if (charIndex >= 32 && charIndex < 128) {
				int charX = (charIndex % 16) * charWidth;
				int charY = (charIndex / 16) * charHeight;

				ML::Box2D scoreBox(startX + x, 20, charWidth, charHeight);
				ML::Box2D src(charX, charY, charWidth, charHeight);

				this->res->scoreFont->Draw(scoreBox, src);
				x += charWidth;
			}
		}

		//---------------------GameOver screen--------------
		//if (true == player->isGameOver) {
		//	int x = ge->screenWidth / 2 - 418 / 2;
		//	int y = ge->screenHeight / 2 - 74 / 2;
		//	ML::Box2D gameOver(x, y, 418, 74);
		//	ML::Box2D srcGO(0, 0, 418, 74);
		//	this->res->imgGameOver->Draw(gameOver, srcGO);

		//	auto startbutton = ge->GetTask<Start::Object>("Start", "NoName");
		//	if (startbutton == nullptr) {
		//		auto startbutton = Start::Object::Create("Start");
		//		startbutton->pos = ML::Box2D(ge->screenWidth / 2 - startbutton->pos.w / 2, ge->screenHeight / 2 + 100, startbutton->pos.w, startbutton->pos.h);
		//	}
		//}
		//--------------------------------------------------
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}