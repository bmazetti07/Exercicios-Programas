import React from 'react'
import { View, StyleSheet, Text, Dimensions, TouchableWithoutFeedback } from 'react-native'
import Entypo from 'react-native-vector-icons/Entypo';
import color from '../misc/color';

const { width } = Dimensions.get('window')

const getThumbnailText = (filename) => {
  return filename[0]
}

const convertTime = (minutes) => {
  if (minutes) {
    const hrs = minutes / 60
    const minute = hrs.toString().split('.')[0]
    let sec = 0
    if (hrs.toString().length > 1) {
      const percent = parseInt(hrs.toString().split('.')[1].slice(0, 2))
      sec = Math.ceil((60 * percent) / 100)
    }

    if (parseInt(minute) < 10 && sec < 10)
      return `0${minute}:0${sec}`

    if (parseInt(minute) < 10)
      return `0${minute}:${sec}`

    if (sec < 10)
      return `${minute}:0${sec}`

    return `${minute}:${sec}`
  }
}

const AudioListItem = ({ title, duration, onOptionPress, onAudioPress }) => {
  return (
    <>
      <View style={styles.container}>
        <TouchableWithoutFeedback onPress={onAudioPress}>
          <View style={styles.leftContainer}>
            <View style={styles.thumbnail}>
              <Text style={styles.thumbnailText}> {getThumbnailText(title)} </Text>
            </View>
            <View style={styles.titleContainer}>
              <Text numberOfLines={1} style={styles.title}> {title} </Text>
              <Text style={styles.timeText}> {convertTime(duration)} </Text>
            </View>
          </View>
        </TouchableWithoutFeedback>

        <View style={styles.rightContainer}>
          <Entypo
            onPress={onOptionPress}
            name="dots-three-vertical"
            size={20}
            color={color.FONT_MEDIUM}
            style={{ padding: 10 }}
          />
        </View>
      </View>

      <View style={styles.separator}></View>
    </>
  )
}

export default AudioListItem

const styles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    alignSelf: 'center',
    width: width - 80,
    marginTop: 10,
  },

  leftContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    flex: 1
  },

  rightContainer: {
    flexBasis: 30,
    height: 50,
    alignItems: 'center',
    justifyContent: 'center',
  },

  thumbnail: {
    height: 50,
    flexBasis: 50,
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: 25,
    backgroundColor: 'grey',
  },

  thumbnailText: {
    fontSize: 22,
    fontWeight: 'bold',
    color: color.FONT
  },

  titleContainer: {
    width: width - 180,
    paddingLeft: 10,
  },

  title: {
    fontSize: 16,
    color: '#fff'
  },

  separator: {
    width: width - 80,
    backgroundColor: color.FONT_LIGHT,
    opacity: 0.3,
    height: 0.5,
    alignSelf: 'center',
    marginTop: 10,
  },

  timeText: {
    fontSize: 14,
    color: color.FONT_LIGHT,
    opacity: 0.4
  }
})