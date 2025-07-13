//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "sound.h"
#include  "Task_Player.h"
#include  "Task_Game.h"
#include  "Task_Apple.h"


namespace Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgPlayer = DG::Image::Create("./data/image/chara.png");
		se::LoadFile("GameOver", "./data/sound/se/mixkit-arcade-retro-game-over-213.wav");
		se::SetVolume("GameOver", 100);

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgPlayer.reset();
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
		this->frameCnt = 0;
		this->spriteIndexWalk = 0;
		this->spriteIndexFacing = 0;
		this->controller = ge->in1;
		this->pos.x = 400;
		this->pos.y = 300;
		this->speed = 3;
		this->points = 0;
		this->hitBase = ML::Box2D(-32, -32, 64, 64);
		this->hitFeet = ML::Box2D(-32, -16, 64, 32);
		this->angle_UDLR = Angle_UDLR::Down;
		this->isGameOver = false;
		this->isSEPlayed = false;
		
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

		//プレイヤ歩きアニメーション
		this->frameCnt = (this->frameCnt + 1) % 40;
		this->spriteIndexWalk = walkPattern[(this->frameCnt / 10 ) % patternLength];


		this->hitBase.x = this->pos.x;
		this->hitBase.y = this->pos.y;
		this->hitFeet.x = this->pos.x;
		this->hitFeet.y = this->pos.y + 32;

		//プレイヤ移動
		if (this->controller) {
			auto  inp = this->controller->GetState();
			if		(inp.LStick.BL.on)	{ this->angle_UDLR = Angle_UDLR::Left;	}
			else if (inp.LStick.BR.on)	{ this->angle_UDLR = Angle_UDLR::Right;	}
			else if (inp.LStick.BU.on)	{ this->angle_UDLR = Angle_UDLR::Up;	}
			else if (inp.LStick.BD.on)	{ this->angle_UDLR = Angle_UDLR::Down;	}
		}
		if (false == CheckBoundaries(this->hitFeet, ge->screenWidth, ge->screenHeight)) {
			switch (this->angle_UDLR) {
			case Angle_UDLR::Left:	this->spriteIndexFacing = 1; this->pos.x -= this->speed; break;
			case Angle_UDLR::Right:	this->spriteIndexFacing = 2; this->pos.x += this->speed; break;
			case Angle_UDLR::Up:	this->spriteIndexFacing = 3; this->pos.y -= this->speed; break;
			case Angle_UDLR::Down:	this->spriteIndexFacing = 0; this->pos.y += this->speed; break;
			}


			this->isGameOver = false; //debug

		}
		else if (true == CheckBoundaries(this->hitFeet, ge->screenWidth, ge->screenHeight)) {
			if (false == this->isSEPlayed) {
				se::Play("GameOver");
				this->isSEPlayed = true;
			}
			this->isGameOver = true;
		}


	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D drawPlayer(0, 0, 64, 64);
		ML::Box2D src(64 * this->spriteIndexWalk, 64 * this->spriteIndexFacing, 64, 64);
		drawPlayer.Offset(pos.x, pos.y);
		this->res->imgPlayer->Draw(drawPlayer, src);
	}

	//-------------------------------------------
	//画面外あたり判定
	bool Object::CheckBoundaries(ML::Box2D hitBox, int x_, int y_) 
	{
		if (hitBox.w + hitBox.x >= x_ || hitBox.x <= 0 || hitBox.h + hitBox.y >= y_ || hitBox.y <= 0) {
			return true;
		}
		else {
			return false;
		}
	}
	//-------------------------------------------
	//オブジェクトあたり判定




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