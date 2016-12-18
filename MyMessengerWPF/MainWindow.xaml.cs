using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;
using ManagedMessenger;

namespace MyMessengerWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        private readonly ManagedMessenger.ManagedMessenger _msg;

        private readonly ObservableCollection<User> _userCollection = new ObservableCollection<User>();

        private readonly Dictionary<string, ObservableCollection<ManagedMessage>> _messengerHistory =
            new Dictionary<string, ObservableCollection<ManagedMessage>>();

        private User _activeConversationUser = null;


        public Data<User> CurrentUser { get; set; } = new Data<User>(null);

        public MainWindow()
        {
            DataContext = this;
            InitializeComponent();
            TextPanel.Visibility = Visibility.Hidden;
            //Item Sources
            ActiveUsers.ItemsSource = _userCollection;
            MessengerHistoryList.ItemsSource = null;
            
            //Initialise messenger
            _msg = new ManagedMessenger.ManagedMessenger("127.0.0.1", 5222);
            _msg.Connect();
            //Subscribe on events
            _msg.OnLogin += Msg_OnLogin;
            _msg.OnUsersRequested += Msg_OnUsersRequested;
            _msg.OnReceivedMessage += Msg_OnReceivedMessage;


           
        }

        private void Msg_OnReceivedMessage(object sender, DataTwoEventArgs<string, ManagedMessage> e)
        {
            var sendUser = e.Data1;
            if (_messengerHistory.ContainsKey(sendUser))
            {
                Dispatcher.BeginInvoke(new Action(()=>_messengerHistory[sendUser].Add(e.Data2)));
            }
        }

        private void Msg_OnUsersRequested(object sender, DataTwoEventArgs<ResultStatus, List<User>> e)
        {
            if (e.Data1 == ResultStatus.Ok)
                this.Dispatcher.BeginInvoke(new Action(() => this.AddUsers(e.Data2)));
        }

        private void AddUsers(List<User> users)
        {
            foreach (var u in users)
                if (!this._userCollection.Contains(u))
                {
                    _userCollection.Add(u);
                    _messengerHistory.Add(u.Username, new ObservableCollection<ManagedMessage>());
                }
        }

        private void Msg_OnLogin(object sender, DataEventArgs<ResultStatus> e)
        {
            if (e.Data == ResultStatus.Ok)
            {
                _msg.BeginRequestUsers();
                //close flyout
                this.Dispatcher.BeginInvoke(
                new Action(() =>
                {
                    LoginFlyout.IsOpen = false;
                    LoggedInCommands.Visibility = Visibility.Visible;
                }));
            }
        }

        private void KeyDownLogin(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string login = Login.Text;
                string password = Password.Password;
                //login function from dll;
                this.CurrentUser.Value = new User(login, password, new SecurityPolicy());
                _msg.BeginLogin(CurrentUser.Value);
            }
        }


        private void LogOutButton_Click(object sender, RoutedEventArgs e)
        {
            _msg.Disconnect();
            this.CurrentUser.Value = null;
            //////////////////////////
            Login.Text = "";
            Password.Clear();
            LoggedInCommands.Visibility = Visibility.Hidden;
            LoginFlyout.IsOpen = true;
        }

        private void SendMessengeButton_Click(object sender, RoutedEventArgs e)
        {
            var msg_text = MessengeBox.Text;
            if (msg_text.Length == 0) return;
            var message = _msg.BeginSend(this._activeConversationUser, msg_text);
            _messengerHistory[_activeConversationUser.Username].Add(message);
            //add message to history
            MessengeBox.Text = "";
        }

        private void MessengeBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                SendMessengeButton_Click(sender, e);
        }

        private void ActiveUsers_SelectionChanged(object sender, SelectionChangedEventArgs e)
        { 
            TextPanel.Visibility = Visibility.Visible;
            _activeConversationUser = (User) ActiveUsers.SelectedItem;
            MessengerHistoryList.ItemsSource = _messengerHistory[_activeConversationUser.Username];
        }


    }
}
