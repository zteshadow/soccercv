
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
print(mnist.train.images.shape, mnist.train.labels.shape)

#with one hidden layer
in_units = 784
h1_units = 300
W1 = tf.Variable(tf.truncated_normal([in_units, h1_units], stddev=0.1))
b1 = tf.Variable(tf.zeros([h1_units]))
W2 = tf.Variable(tf.zeros([h1_units, 10]))
b2 = tf.Variable(tf.zeros([10]))

x = tf.placeholder(tf.float32, [None, in_units])
keep_prob = tf.placeholder(dtype=tf.float32)
hidden1 = tf.nn.relu(tf.matmul(x, W1) + b1)
hidden1_drop = tf.nn.dropout(hidden1, keep_prob)
y = tf.nn.softmax(tf.matmul(hidden1_drop, W2) + b2)

y_ = tf.placeholder(tf.float32, [None, 10])
cost = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices=[1]))

session = tf.InteractiveSession()
path = './model/model.ckpt'
saver = tf.train.Saver()
#saver.restore(session, path)

train_step = tf.train.AdagradOptimizer(0.3).minimize(cost)
tf.global_variables_initializer().run()
for i in range(3000):
    batch_x, batch_y = mnist.train.next_batch(100)
    train_step.run({x:batch_x, y_:batch_y, keep_prob:0.75})
    print('cost[', str(i), ']: ', session.run((cost), feed_dict={x:batch_x, y_:batch_y, keep_prob:0.75}))
    #print('cost[', str(i), ']: ', tf.cast(cost, tf.float32))

saver.save(session, path)
correct = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))
print(accuracy.eval({x:mnist.test.images, y_:mnist.test.labels, keep_prob:1.0}))

