#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("Player");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP imgPlayer;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		XI::GamePad::SP  controller;
		ML::Vec2    pos;		//キャラクタ位置
		ML::Box2D   hitBase;	//あたり判定範囲
		ML::Box2D	hitFeet;
		bool		hitFlag;	//足元判定確認用
		int			speed;
		int			points;

		//上下左右の向き
		enum class Angle_UDLR { Up, Down, Left, Right };
		Angle_UDLR angle_UDLR;

		//プレイヤ歩きアニメーション
		int frameCnt;
		const int walkPattern[4] = { 0, 1, 2, 1 };
		const int patternLength = sizeof(walkPattern) / sizeof(walkPattern[0]);
		int spriteIndexWalk;
		int spriteIndexFacing;

		bool isSEPlayed;
		bool isGameOver;
		bool CheckBoundaries( ML::Box2D hitBox,  int x_,  int y_);
	//	bool CheckHit(const ML::Box2D hitBoxPlayer, const ML::Box2D hitBoxObject);
	};
}