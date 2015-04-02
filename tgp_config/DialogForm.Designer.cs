namespace tgp_config {
	partial class DialogForm {
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
			this.panel1 = new System.Windows.Forms.Panel();
			this.fullscreenRadioButton = new System.Windows.Forms.RadioButton();
			this.windowedRadioButton = new System.Windows.Forms.RadioButton();
			this.windowResolutionList = new System.Windows.Forms.ListBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.kbdMoveComboBox = new System.Windows.Forms.ComboBox();
			this.joyMoveComboBox = new System.Windows.Forms.ComboBox();
			this.kbdLRotateComboBox = new System.Windows.Forms.ComboBox();
			this.joyLRotateComboBox = new System.Windows.Forms.ComboBox();
			this.kbdRRotateComboBox = new System.Windows.Forms.ComboBox();
			this.joyRRotateComboBox = new System.Windows.Forms.ComboBox();
			this.kbdActionComboBox = new System.Windows.Forms.ComboBox();
			this.joyActionComboBox = new System.Windows.Forms.ComboBox();
			this.kbdSubActComboBox = new System.Windows.Forms.ComboBox();
			this.joySubActComboBox = new System.Windows.Forms.ComboBox();
			this.groupBox1.SuspendLayout();
			this.flowLayoutPanel1.SuspendLayout();
			this.panel1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.AutoSize = true;
			this.groupBox1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.groupBox1.Controls.Add(this.flowLayoutPanel1);
			this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.groupBox1.Location = new System.Drawing.Point(0, 0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(322, 88);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "画面解像度";
			// 
			// flowLayoutPanel1
			// 
			this.flowLayoutPanel1.AutoSize = true;
			this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.flowLayoutPanel1.Controls.Add(this.panel1);
			this.flowLayoutPanel1.Controls.Add(this.windowResolutionList);
			this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 15);
			this.flowLayoutPanel1.Name = "flowLayoutPanel1";
			this.flowLayoutPanel1.Size = new System.Drawing.Size(316, 70);
			this.flowLayoutPanel1.TabIndex = 0;
			// 
			// panel1
			// 
			this.panel1.AutoSize = true;
			this.panel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.panel1.Controls.Add(this.fullscreenRadioButton);
			this.panel1.Controls.Add(this.windowedRadioButton);
			this.panel1.Location = new System.Drawing.Point(3, 3);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(103, 44);
			this.panel1.TabIndex = 0;
			// 
			// fullscreenRadioButton
			// 
			this.fullscreenRadioButton.AutoSize = true;
			this.fullscreenRadioButton.Location = new System.Drawing.Point(6, 25);
			this.fullscreenRadioButton.Name = "fullscreenRadioButton";
			this.fullscreenRadioButton.Size = new System.Drawing.Size(84, 16);
			this.fullscreenRadioButton.TabIndex = 2;
			this.fullscreenRadioButton.TabStop = true;
			this.fullscreenRadioButton.Text = "フルスクリーン";
			this.fullscreenRadioButton.UseVisualStyleBackColor = true;
			// 
			// windowedRadioButton
			// 
			this.windowedRadioButton.AutoSize = true;
			this.windowedRadioButton.Location = new System.Drawing.Point(6, 3);
			this.windowedRadioButton.Name = "windowedRadioButton";
			this.windowedRadioButton.Size = new System.Drawing.Size(94, 16);
			this.windowedRadioButton.TabIndex = 1;
			this.windowedRadioButton.TabStop = true;
			this.windowedRadioButton.Text = "ウィンドウモード";
			this.windowedRadioButton.UseVisualStyleBackColor = true;
			// 
			// windowResolutionList
			// 
			this.windowResolutionList.FormattingEnabled = true;
			this.windowResolutionList.ItemHeight = 12;
			this.windowResolutionList.Location = new System.Drawing.Point(112, 3);
			this.windowResolutionList.Name = "windowResolutionList";
			this.windowResolutionList.Size = new System.Drawing.Size(108, 64);
			this.windowResolutionList.TabIndex = 0;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.tableLayoutPanel1);
			this.groupBox2.Dock = System.Windows.Forms.DockStyle.Top;
			this.groupBox2.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.groupBox2.Location = new System.Drawing.Point(0, 88);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(322, 157);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "コントローラ";
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 3;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Controls.Add(this.label1, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.label2, 2, 0);
			this.tableLayoutPanel1.Controls.Add(this.label3, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.label4, 0, 2);
			this.tableLayoutPanel1.Controls.Add(this.label5, 0, 3);
			this.tableLayoutPanel1.Controls.Add(this.label6, 0, 4);
			this.tableLayoutPanel1.Controls.Add(this.label7, 0, 5);
			this.tableLayoutPanel1.Controls.Add(this.kbdMoveComboBox, 1, 1);
			this.tableLayoutPanel1.Controls.Add(this.joyMoveComboBox, 2, 1);
			this.tableLayoutPanel1.Controls.Add(this.kbdLRotateComboBox, 1, 2);
			this.tableLayoutPanel1.Controls.Add(this.joyLRotateComboBox, 2, 2);
			this.tableLayoutPanel1.Controls.Add(this.kbdRRotateComboBox, 1, 3);
			this.tableLayoutPanel1.Controls.Add(this.joyRRotateComboBox, 2, 3);
			this.tableLayoutPanel1.Controls.Add(this.kbdActionComboBox, 1, 4);
			this.tableLayoutPanel1.Controls.Add(this.joyActionComboBox, 2, 4);
			this.tableLayoutPanel1.Controls.Add(this.kbdSubActComboBox, 1, 5);
			this.tableLayoutPanel1.Controls.Add(this.joySubActComboBox, 2, 5);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 15);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 6;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(316, 139);
			this.tableLayoutPanel1.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(119, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(54, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "キーボード";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(219, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(61, 12);
			this.label2.TabIndex = 1;
			this.label2.Text = "ゲームパッド";
			// 
			// label3
			// 
			this.label3.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(84, 18);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 12);
			this.label3.TabIndex = 2;
			this.label3.Text = "移動";
			// 
			// label4
			// 
			this.label4.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(72, 43);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(41, 12);
			this.label4.TabIndex = 2;
			this.label4.Text = "左旋回";
			// 
			// label5
			// 
			this.label5.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(72, 68);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(41, 12);
			this.label5.TabIndex = 2;
			this.label5.Text = "右旋回";
			// 
			// label6
			// 
			this.label6.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(36, 93);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(77, 12);
			this.label6.TabIndex = 2;
			this.label6.Text = "アクション/決定";
			// 
			// label7
			// 
			this.label7.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(3, 119);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(110, 12);
			this.label7.TabIndex = 2;
			this.label7.Text = "モード切替/キャンセル";
			// 
			// kbdMoveComboBox
			// 
			this.kbdMoveComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.kbdMoveComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.kbdMoveComboBox.FormattingEnabled = true;
			this.kbdMoveComboBox.Location = new System.Drawing.Point(119, 15);
			this.kbdMoveComboBox.Name = "kbdMoveComboBox";
			this.kbdMoveComboBox.Size = new System.Drawing.Size(94, 20);
			this.kbdMoveComboBox.TabIndex = 3;
			// 
			// joyMoveComboBox
			// 
			this.joyMoveComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.joyMoveComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.joyMoveComboBox.FormattingEnabled = true;
			this.joyMoveComboBox.Location = new System.Drawing.Point(219, 15);
			this.joyMoveComboBox.Name = "joyMoveComboBox";
			this.joyMoveComboBox.Size = new System.Drawing.Size(94, 20);
			this.joyMoveComboBox.TabIndex = 3;
			// 
			// kbdLRotateComboBox
			// 
			this.kbdLRotateComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.kbdLRotateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.kbdLRotateComboBox.FormattingEnabled = true;
			this.kbdLRotateComboBox.Location = new System.Drawing.Point(119, 40);
			this.kbdLRotateComboBox.Name = "kbdLRotateComboBox";
			this.kbdLRotateComboBox.Size = new System.Drawing.Size(94, 20);
			this.kbdLRotateComboBox.TabIndex = 3;
			// 
			// joyLRotateComboBox
			// 
			this.joyLRotateComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.joyLRotateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.joyLRotateComboBox.FormattingEnabled = true;
			this.joyLRotateComboBox.Location = new System.Drawing.Point(219, 40);
			this.joyLRotateComboBox.Name = "joyLRotateComboBox";
			this.joyLRotateComboBox.Size = new System.Drawing.Size(94, 20);
			this.joyLRotateComboBox.TabIndex = 3;
			// 
			// kbdRRotateComboBox
			// 
			this.kbdRRotateComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.kbdRRotateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.kbdRRotateComboBox.FormattingEnabled = true;
			this.kbdRRotateComboBox.Location = new System.Drawing.Point(119, 65);
			this.kbdRRotateComboBox.Name = "kbdRRotateComboBox";
			this.kbdRRotateComboBox.Size = new System.Drawing.Size(94, 20);
			this.kbdRRotateComboBox.TabIndex = 3;
			// 
			// joyRRotateComboBox
			// 
			this.joyRRotateComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.joyRRotateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.joyRRotateComboBox.FormattingEnabled = true;
			this.joyRRotateComboBox.Location = new System.Drawing.Point(219, 65);
			this.joyRRotateComboBox.Name = "joyRRotateComboBox";
			this.joyRRotateComboBox.Size = new System.Drawing.Size(94, 20);
			this.joyRRotateComboBox.TabIndex = 3;
			// 
			// kbdActionComboBox
			// 
			this.kbdActionComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.kbdActionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.kbdActionComboBox.FormattingEnabled = true;
			this.kbdActionComboBox.Location = new System.Drawing.Point(119, 90);
			this.kbdActionComboBox.Name = "kbdActionComboBox";
			this.kbdActionComboBox.Size = new System.Drawing.Size(94, 20);
			this.kbdActionComboBox.TabIndex = 3;
			// 
			// joyActionComboBox
			// 
			this.joyActionComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.joyActionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.joyActionComboBox.FormattingEnabled = true;
			this.joyActionComboBox.Location = new System.Drawing.Point(219, 90);
			this.joyActionComboBox.Name = "joyActionComboBox";
			this.joyActionComboBox.Size = new System.Drawing.Size(94, 20);
			this.joyActionComboBox.TabIndex = 3;
			// 
			// kbdSubActComboBox
			// 
			this.kbdSubActComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.kbdSubActComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.kbdSubActComboBox.FormattingEnabled = true;
			this.kbdSubActComboBox.Location = new System.Drawing.Point(119, 115);
			this.kbdSubActComboBox.Name = "kbdSubActComboBox";
			this.kbdSubActComboBox.Size = new System.Drawing.Size(94, 20);
			this.kbdSubActComboBox.TabIndex = 3;
			// 
			// joySubActComboBox
			// 
			this.joySubActComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.joySubActComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.joySubActComboBox.FormattingEnabled = true;
			this.joySubActComboBox.Location = new System.Drawing.Point(219, 115);
			this.joySubActComboBox.Name = "joySubActComboBox";
			this.joySubActComboBox.Size = new System.Drawing.Size(94, 20);
			this.joySubActComboBox.TabIndex = 3;
			// 
			// DialogForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(322, 293);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DialogForm";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.DialogForm_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.flowLayoutPanel1.ResumeLayout(false);
			this.flowLayoutPanel1.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton fullscreenRadioButton;
		private System.Windows.Forms.RadioButton windowedRadioButton;
		private System.Windows.Forms.ListBox windowResolutionList;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.ComboBox kbdMoveComboBox;
		private System.Windows.Forms.ComboBox joyMoveComboBox;
		private System.Windows.Forms.ComboBox kbdLRotateComboBox;
		private System.Windows.Forms.ComboBox joyLRotateComboBox;
		private System.Windows.Forms.ComboBox kbdRRotateComboBox;
		private System.Windows.Forms.ComboBox joyRRotateComboBox;
		private System.Windows.Forms.ComboBox kbdActionComboBox;
		private System.Windows.Forms.ComboBox joyActionComboBox;
		private System.Windows.Forms.ComboBox kbdSubActComboBox;
		private System.Windows.Forms.ComboBox joySubActComboBox;
		private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
		private System.Windows.Forms.Panel panel1;

	}
}

