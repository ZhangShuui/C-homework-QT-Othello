#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 导入随机包
import random
import numpy as np
import datetime
import math
import copy
from game import Game
from board import Board


class RandomPlayer:
    """
    随机玩家, 随机返回一个合法落子位置
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """
        self.color = color

    def random_choice(self, board):
        """
        从合法落子位置中随机选一个落子位置
        :param board: 棋盘
        :return: 随机合法落子位置, e.g. 'A1'
        """
        # 用 list() 方法获取所有合法落子位置坐标列表
        action_list = list(board.get_legal_actions(self.color))

        # 如果 action_list 为空，则返回 None,否则从中选取一个随机元素，即合法的落子坐标
        if len(action_list) == 0:
            return None
        else:
            return random.choice(action_list)

    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
        else:
            player_name = '白棋'
        print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))
        action = self.random_choice(board)
        return action


class HumanPlayer:
    """
    人类玩家
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """
        self.color = color

    def get_move(self, board):
        """
        根据当前棋盘输入人类合法落子位置
        :param board: 棋盘
        :return: 人类下棋落子位置
        """
        # 如果 self.color 是黑棋 "X",则 player 是 "黑棋"，否则是 "白棋"
        if self.color == "X":
            player = "黑棋"
        else:
            player = "白棋"

        # 人类玩家输入落子位置，如果输入 'Q', 则返回 'Q'并结束比赛。
        # 如果人类玩家输入棋盘位置，e.g. 'A1'，
        # 首先判断输入是否正确，然后再判断是否符合黑白棋规则的落子位置
        while True:
            action = input(
                "请'{}-{}'方输入一个合法的坐标(e.g. 'D3'，若不想进行，请务必输入'Q'结束游戏。): ".format(player,
                                                                             self.color))

            # 如果人类玩家输入 Q 则表示想结束比赛
            if action == "Q" or action == 'q':
                return "Q"
            else:
                row, col = action[1].upper(), action[0].upper()

                # 检查人类输入是否正确
                if row in '12345678' and col in 'ABCDEFGH':
                    # 检查人类输入是否为符合规则的可落子位置
                    if action in board.get_legal_actions(self.color):
                        return action
                else:
                    print("你的输入不合法，请重新输入!")


import random
import numpy as np
import datetime
import math
import copy


class Node:
    def __init__(self, board=None, parent=None, color=None):
        self.board = board
        self.parent = parent
        self.child = []
        self.color = color
        self.N = 0
        self.Q = 0
        self.expanded = False

    def ucb_worth(self, one, search_time):
        return one * self.Q / self.N + 1.1 * math.sqrt(2 * math.log(search_time) / self.N)

    def all_child_expanded(self):
        if self.child is None or len(self.child) < 1:
            return False

        for ch_node in self.child:
            if not ch_node.expanded:
                return False
        return True

    def add_child(self):
        col = "X"
        if self.color == "X":
            col = "O"
        v_set = list(self.board.get_legal_actions(self.color))
        if not v_set:
            return
        for point in v_set:
            bo = copy.deepcopy(self.board)
            bo._move(point, self.color)
            newnode = Node(bo, self, col)
            self.child.append(newnode)

    def q(self, color):
        if color == "X":
            col = 0
        else:
            col = 1
        if self.board.get_winner()[0] == col:
            return 1 + self.board.get_winner()[1] / 64.0
        elif self.board.get_winner()[0] == 2:
            return 0.5
        else:
            return 0


class UCBTree:
    def __init__(self, board=None):
        self.board = board
        self.root = Node()


class AIPlayer:
    """
    AI 玩家
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """
        self.ucb_tree = UCBTree()
        self.color = color
        self.search_time = 0

    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
        else:
            player_name = '白棋'
        print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))

        # -----------------请实现你的算法代码--------------------------------------

        action = self.uct_search(board)
        # ------------------------------------------------------------------------
        return action

    def game_over(self, board):
        b_list = list(board.get_legal_actions('X'))
        w_list = list(board.get_legal_actions('O'))

        is_over = len(b_list) == 0 and len(w_list) == 0
        return is_over

    def uct_search(self, board):
        v0 = Node(board, color=self.color)
        self.ucb_tree.root = v0

        start = datetime.datetime.now()
        end = datetime.datetime.now()
        while (end - start).seconds < 550:
            v_select = self.select_policy(v0)
            v_expand = self.expand(v_select)
            quality = self.simulate_policy(v_expand)
            self.back_propagate(quality, v_expand)
            end = datetime.datetime.now()

        maxi = 0
        for i in range(0, len(v0.child)):
            if v0.child[i].Q > v0.child[maxi].Q:
                maxi = i
        action = list(board.get_legal_actions(self.color))[maxi]
        return action

    def select_policy(self, v0):
        v = v0
        one = 1
        while v.expanded and v.all_child_expanded():
            v = self.UCB(v, one)
            one = -one
        return v

    def UCB(self, v, one):
        v_choice = v.child[0]
        for v_child in v.child:
            if v_child.ucb_worth(one, self.search_time) > v_choice.ucb_worth(one, self.search_time):
                v_choice = v_child
        return v_choice

    def expand(self, v_select):
        if v_select is self.ucb_tree.root:
            v_select.expanded = True
            if not v_select.child:
                v_select.add_child()
            for chi_node in v_select.child:
                if not chi_node.expanded:
                    thenode = chi_node
                    break
            thenode.expanded = True
            thenode.add_child()
            return thenode
        if not v_select.child:
            return v_select
        the_node = None
        for chi_node in v_select.child:
            if not chi_node.expanded:
                the_node = chi_node
                break
        the_node.expanded = True
        the_node.add_child()
        return the_node

    def simulate_policy(self, v_expand):
        if not v_expand.child:
            quality = v_expand.q(self.color)
            return quality
        quality = 0
        for i in range(0, 10):
            if v_expand.color == "X":
                second_color = "X"
            else:
                second_color = "O"
            board = copy.deepcopy(v_expand.board)

            while not self.game_over(board):
                actions = list(board.get_legal_actions(second_color))

                if len(actions) > 0:
                    board._move(actions[random.randint(0, len(actions) - 1)], second_color)
                if second_color == "X":
                    second_color = "O"
                else:
                    second_color = "X"
            node = Node(board, None, v_expand.color)
            quality += node.q(self.color)
        quality /= 10.0
        return quality

    def back_propagate(self, quality, the):
        while the.parent is not None:
            the.Q += quality
            the.N += 1
            the = the.parent
            self.search_time += 1
        the.Q += quality
        the.N += 1


#black_player = HumanPlayer("X")

## AI 玩家 白棋初始化
#white_player = AIPlayer("O")

## 游戏初始化，第一个玩家是黑棋，第二个玩家是白棋
#game = Game(black_player, white_player)

## 开始下棋
#game.run()
