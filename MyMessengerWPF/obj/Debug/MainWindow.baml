   M S B A M L   `   `   `  ����T  PMahApps.Metro, Version=1.3.0.0, Culture=neutral, PublicKeyToken=f4fb5a3c4d1e5b4fV;clr-namespace:MahApps.Metro.Controls;assembly=MahApps.MetroMahApps.Metro.Controls  5   	   I EMyMessengerWPF, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null/clr-namespace:MyMessengerWPFMyMessengerWPF 5   	   Z=clr-namespace:MahApps.Metro.Behaviours;assembly=MahApps.MetroMahApps.Metro.Behaviours  5	         MyMessengerWPF.MainWindow   5      HControls;clr-namespace:MahApps.Metro.Controls;assembly=MahApps.Metro  5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   .localclr-namespace:MyMessengerWPF     5   	   c _System.Windows.Interactivity, Version=4.5.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35Bi:http://schemas.microsoft.com/expression/2010/interactivity  5   	   N
Behaviours=clr-namespace:MahApps.Metro.Behaviours;assembly=MahApps.Metro  5	      -   5        (   "MahApps.Metro.Controls.MetroWindow   Name$  MyMessenger��5       � Title$ Kaspersky Messenger��5   	   $	��350��6%   $	��525��62   $	��350��6>   $	��525��6N   .  (System.Windows.Interactivity.Interaction   	Behaviors 5      7   1MahApps.Metro.Behaviours.BorderlessWindowBehavior  5   
      RightWindowCommands 5      +   %MahApps.Metro.Controls.WindowCommands  5   
   .@�5      �� -   5         ��Name$ LogOutButton��5      $��Log-Out��6+   $y�Hidden=�6=   6Q   5         Flyouts 5      +   %MahApps.Metro.Controls.FlyoutsControl  5   
   .@�5      #   MahApps.Metro.Controls.Flyout  -   5          Name$ LoginFlyout��5      $��Bottom=�63   $��Stretch=�6N      Position$	 top=�6l      IsOpen . 	   CloseButtonVisibility$	 	Collapsed=�5     ��6   �� 
 �� RelativeSource
 �� *FindAncestor+ �� AncestorType$ Controls:MetroWindowL� �� Path$ ActualWidth���6�   �� 
 �� *FindAncestor+$ Controls:MetroWindowL�$ ActualHeight�.��5      � -   5         �Name$ MainGrid��5      ���	#FF151414$��Stretch=�6@   $��Stretch=�6]    � RowDefinitions 5      �� 5      $	��76* �6(   �� 5      $
��243* �6(    � ColumnDefinitions 5      �� 5       $	��80* �6+   �� 5!      $
��337* �6+   �� 5"      $	��80* �6+   .+�5$      �� -   5         ��Name$ Login��5$   u   *   $MahApps.Metro.Controls.TextBoxHelper   ClearTextButton .    	Watermark$ Login��6L   $��
81,58,72,0q�6�    �� TextWrapping$
 Wrap=�6�   
�������$��1��6�   $��29��6�   $	��Top=�6�   $��1��6�   $��16(�6    � ToolTip$ Login��6  
���   ����	#FF0E0D0D61  �� 5%      -   5         ��Name$ Log_In��5%      $��Log-In��6,   $��135,2,142,0q�6=   $��	DokChampa)�6R   
�������$��18(�6|   $��1��6�   $��34��6�   $	��Top=�6�   $��1��6�   $��90��6�   G� 5&      -   5         G�Name$ Password��5&   |    . $ Password��6P   $��81,104,72,0q�6�   
�������$��1��6�   $��29��6�   $	��Top=�6�   $��1��6�   $��16(�6�   $ Password��6  
���   ����	#FF0808086,  5'      5(      5)      .��5.      � -   5        $ WindowsGrid��5.      .+�5/   
   � -	   5        $ 
SecondGrid��5/      ���	#FF151414$��Stretch=�6:   $��Stretch=�6W    50       52      �� 53      $
��119* �6#   �� 54      $
��398* �6#   .+�56      � -
   5        $ 	TextPanel��56      $��1��6&    57      �� 58      $
��258* �6$   �� 59      $	��61* �6$   .+�5;      �� -   5         ��Name$ MessengerHistoryList��5;      $��0,0,0,1q�68   ���	#FF131313
���   ���5<      �� 5=      .R�5>      �� .+�5?   "   �� ��6,   �� 5@      5A      5C      � 5D      -   5        $ MessengerPanel��5D      ���	#FF0C0C0C$��1��6F    5E      �� 5F      $
��170* �6+   �� 5G      $	��29* �6+   .+�5I      �� -   5        $ MessengeBox��5I      $! Write here your messenge...��63   $��	10,10,0,7q�6r   $
 Wrap=�6�   ���	#FF171717
�������6�   �� 5J      -   5        $ SendMessengeButton��5J         MetroCircleButtonStyle#���   69   $
��Send��6j   $��5,10,5,7q�6y   $��1��6�   $��Center=�6�   $��Center=�6�   $��44��6�   $��48��6�   6�   5K      5L      � 5M      -   5        $ 	LeftPanel��5M      ���	#FF0F0F0F.+�5N      �� -   5        $ ActiveUsers��5N      $��	0,49,0,28q�6/   ��5O      �� 5P      .R�5Q      �� ��6(   �� *
Username+5R      5T      �� 5U      -   5        $ label��5U      $��USERS��6'   $��33��67   $��
10,10,10,0q�6C   $	��Top=�6W   $
��Bold"�6o   $��16(�6�   �� 5V      -   5         ��Name$ LeftPannelBorder��5V      ���	#FF686464$��1q�6K   $��0,0,-1,0q�6_   5W      5X      5Y      5Z      