//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  <time.h> 
#include  "sound.h"
#include  "Task_Apple.h"
#include  "Task_Player.h"

namespace Apple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgApple = DG::Image::Create("./data/image/fruit_ringo.png");

		se::LoadFile("AppleGet","./data/sound/se/poku.wav");
		se::SetVolume("AppleGet", 100);

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgApple.reset();
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

		//★データ初期化
	//	int posxmax = ge->screenWidth - 32;
	//	int posymax = ge->screenHeight - 32;
	//	this->pos = /*ML::Vec2(150, 150);  */ ML::Vec2(rand() % posxmax, rand() % posymax);

		this->pos = ML::Vec2(10, 10);

		this->hitBase = ML::Box2D(-16 + this->pos.x, -16 + this->pos.y, 32, 32);
		
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
		

		this->hitBase.x = this->pos.x;
		this->hitBase.y = this->pos.y;

		//------------------------------------------------------------------
		//------------------あたり判定---------------------------------------
		ML::Box2D me = this->hitBase;

		//auto player = ge->GetTask<Player::Object>("Player", "NoName");
		//ML::Box2D you = player->hitBase.OffsetCopy(player->pos);

		//if (true == me.Hit(player->hitBase)) {
		//	player->points += 1;
		//	player->speed += 1;
		//	se::Play("AppleGet");
		//	this->Kill();
		//}
		//------------------------------------------------------------------

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 32, 32);
		ML::Box2D src(0, 0, 400, 400);
		draw.Offset(pos.x, pos.y);
		this->res->imgApple->Draw(draw, src);


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