import firebase from 'firebase'
import 'firebase/storage'

var firebaseConfig = {
    apiKey: "AIzaSyACw_n4yxcdAklGUuWCsc9IYdkUVBFRw7U",
    authDomain: "ep1musicplayer.firebaseapp.com",
    projectId: "ep1musicplayer",
    storageBucket: "ep1musicplayer.appspot.com",
    messagingSenderId: "1038132406903",
    appId: "1:1038132406903:web:650ab9011abe3230373472"
};

firebase.initializeApp(firebaseConfig)
export default firebase