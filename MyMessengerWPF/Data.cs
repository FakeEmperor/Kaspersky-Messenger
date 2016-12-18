using System.Collections.Generic;
using System.ComponentModel;

namespace MyMessengerWPF
{   
    public class Data<T> : INotifyPropertyChanged
    {
        // props
        private T _data;

        public Data(T data)
        {
            _data = data;
        }

        // boiler-plate
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        protected bool SetField(T value)
        {
            if (EqualityComparer<T>.Default.Equals(_data, value)) return false;
            _data = value;
            OnPropertyChanged("Value");
            return true;
        }

        public T Value
        {
            get { return _data; }
            set { SetField(value); }
        }
    }
}