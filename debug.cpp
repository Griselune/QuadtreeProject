//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "debug.h"
#include  "Task_Player.h"
#include  "Task_Apple.h"

namespace debug
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgDebugRectPlayer = DG::Image::Create("./data/image/debugrect.png");
		this->imgDebugRectApple = DG::Image::Create("./data/image/debugrect.png");
		this->font = DG::Font::Create("HG丸ゴシックM-PRO", 8, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgDebugRectPlayer.reset();
		this->imgDebugRectApple.reset();
		this->font.reset();
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
		this->isOn = false;
		this->hitFlag = false;
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//if (auto pl = this->player.lock()) {

		//}
		//if (auto ap = this->apple.lock()) {

		//}


	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (true == isOn) {

			auto player = ge->GetTask<Player::Object>("Player", "NoName");
			auto apple = ge->GetTask<Apple::Object>("Apple", "NormalApple");

			if (player != nullptr) {
				ML::Box2D drawRectPlayer = player->hitBase;
				ML::Box2D src(64 + 32 * this->hitFlag, 32, 32, 32);
				this->res->imgDebugRectPlayer->Draw(drawRectPlayer, src);

				ML::Box2D drawRectPlayerFeet = player->hitFeet;
				ML::Box2D srcFeet(64 + 32 * this->hitFlag, 0, 32, 32);
				this->res->imgDebugRectPlayer->Draw(drawRectPlayerFeet, srcFeet);
			}

			if (apple != nullptr) {
				ML::Box2D drawRectApple = apple->hitBase;
				ML::Box2D src(64 + 32 * this->hitFlag, 32, 32, 32);
				this->res->imgDebugRectApple->Draw(drawRectApple, src);
			}

			if (player != nullptr) {
				ML::Box2D textBox(0, 0, 480, 720);
				string text =
					"ScreenSizeX: " + to_string(ge->screenWidth) + "\n" +
					"ScreenSizeY: " + to_string(ge->screenHeight) + "\n" +
					"Player PosX: " + to_string(player->pos.x) + "\n" +
					"Player PosY: " + to_string(player->pos.y) + "\n" +
					"Player feet X: " + to_string(player->hitFeet.x) + "\n" +
					"Player feet Y: " + to_string(player->hitFeet.y) + "\n" +
					"Player feet W+X: " + to_string(player->hitFeet.w + player->hitFeet.x) + "\n" +
					"Player feet H+Y: " + to_string(player->hitFeet.h + player->hitFeet.y);
				this->res->font->Draw(textBox, text, ML::Color(1.0f, 0.0f, 0.0f, 0.0f));
			}
		}
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